#include "syntax_analyzer.h"
#include <unistd.h>
#include <future>
#include <iostream>
#include "glog/logging.h"

const int SyntaxAnalyzer::kMaxThreadNum = 6;
const int SyntaxAnalyzer::kTokenMaxSize = 100;

SyntaxAnalyzer::SyntaxAnalyzer() {
  Json::StreamWriterBuilder builer;
  writer_ = std::unique_ptr<Json::StreamWriter>(builer.newStreamWriter());
  clang_analyzer_ = std::make_unique<ClangAnalyzer>();
}

void SyntaxAnalyzer::processRequest(const Json::Value& request) {
  std::shared_ptr<Request> req = std::make_shared<Request>();

  std::cerr << "Process Begin" << std::endl;

  req->translation_unit_name = request[1]["filename"].asString();
  req->parse_id = request[1]["cnt"].asInt();
  req->buf_num = request[1]["bufnr"].asInt();

  UnsavedFile unsaved;
  unsaved.filename_ = request[1]["unsaved"]["filename"].asString();
  unsaved.contents_ = request[1]["unsaved"]["content"].asString();
  unsaved.length_ = unsaved.contents_.size();

  req->unsaved_files = std::make_shared<std::vector<UnsavedFile>>();
  req->unsaved_files->emplace_back(unsaved);

  req->flags = std::make_shared<std::vector<std::string>>();
  for (auto const& flag : request[1]["flags"]) {
    req->flags->emplace_back(flag.asString());
  }

  // check if we have thread to process the request
  bool should_emit = false;
  access_lock_.lock();
  if (thread_number_ < kMaxThreadNum) {
    thread_number_++;
    should_emit = true;
    buf_parse_id_[req->buf_num] = req->parse_id;
    std::cerr << "Thread number++:" << thread_number_ << std::endl;
  }
  access_lock_.unlock();

  if (should_emit) {
    std::thread(
        [this](const std::shared_ptr<Request>& emit) {
          std::cerr << "Parsing begin! Id:" << emit->parse_id << " ,Bufnr:" << emit->buf_num
                    << " ,File:" << emit->translation_unit_name << std::endl;

          Json::Value response;
          response[0] = 0;
          response[1]["cnt"] = emit->parse_id;
          response[1]["filename"] = emit->translation_unit_name;
          response[1]["bufnr"] = emit->buf_num;

          auto highlights = clang_analyzer_->UpdateTranslationUnit(
              emit->translation_unit_name, *(emit->unsaved_files), *(emit->flags.get()));

          if (highlights != nullptr) {
            std::cerr << emit->parse_id << " Hightlight original size:" << highlights->size()
                      << std::endl;
            std::unordered_map<std::string, int> text_pool;
            for (const auto& highlight : *highlights) {
              if (text_pool.find(highlight.text_) == text_pool.end()) {
                text_pool[highlight.text_] = 0;
              }
              if ((text_pool[highlight.text_]++) < kTokenMaxSize) {
                Json::Value value;
                value["text"] = highlight.text_;
                value["type"] = highlight.type_;
                value["line"] = highlight.line_;
                value["col"] = highlight.col_;
                response[1]["highlights"].append(value);
                // std::cerr<<"Hightlight:"<<highlight.text_<<" , "<<highlight.line_<<" ,
                // "<<highlight.col_<<" , "<<highlight.type_<<std::endl;
              }
            }
          } else {
            std::cerr << "Highlights is nullptr" << std::endl;
          }

          std::cerr << emit->parse_id
                    << " Hightlight actual size:" << response[1]["highlights"].size() << std::endl;

          // check if we shoud reponse, cause new parse request on same file may have arrive when we
          // parsing, if so, the parsing result should drop
          bool should_response = false;
          access_lock_.lock();
          int highlight_size = response[1]["highlights"].size();
          should_response = (emit->parse_id >= buf_parse_id_[emit->buf_num]) && highlight_size > 0;

          // if highlight is zero, we should set the parse_id to -1, so the subsequent reponse can
          // send, though they may have lower parse_id
          if (highlight_size <= 0) {
            buf_parse_id_[emit->buf_num] = -1;
          }
          access_lock_.unlock();

          if (should_response) {
            stdout_lock_.lock();
            writer_->write(response, &std::cout);
            stdout_lock_.unlock();
          }

          std::cerr << "Parsing finish! Id:" << emit->parse_id << " ,Bufnr:" << emit->buf_num
                    << " ,File:" << emit->translation_unit_name << std::endl;

          // decrease the thread_number_ so other request can by emit
          access_lock_.lock();
          thread_number_--;
          std::cerr << "Thread number--:" << thread_number_ << std::endl;
          access_lock_.unlock();
        },
        req)
        .detach();
  } else {
    std::cerr << "Not enough thread, request dropped!!!" << std::endl;
  }
  std::cerr << "Process Done" << std::endl;
}
