#include "syntax_analyzer.h"
#include "glog/logging.h"
#include <future>
#include <iostream>
#include <unistd.h>

const int SyntaxAnalyzer::kMaxThreadNum = 6;

SyntaxAnalyzer::SyntaxAnalyzer() {
  Json::StreamWriterBuilder builer;
  writer_ = std::unique_ptr<Json::StreamWriter>(builer.newStreamWriter());
  clang_analyzer_ = std::make_unique<ClangAnalyzer>();
}

void SyntaxAnalyzer::processRequest(const Json::Value &request) {
  std::shared_ptr<Request> req = std::make_shared<Request>();

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
  for (auto const &flag : request[1]["flags"]) {
    req->flags->emplace_back(flag.asString());
  }

  // check if we have thread to process the request
  bool should_emit = false;
  thread_counter_lock_.lock();
  if(thread_number_ < kMaxThreadNum){
    thread_number_++;
    should_emit = true;
    LOG(INFO)<<"Thread number++:"<<thread_number_;
  }
  thread_counter_lock_.unlock();

  if(should_emit){
    std::async(std::launch::async, [this](const std::shared_ptr<Request>& emit) {
      LOG(INFO)<<"Parsing begin! Id:"<<emit->parse_id<<" ,Bufnr:"<<emit->buf_num<<" ,File:"<<emit->translation_unit_name;

      auto highlights = clang_analyzer_->UpdateTranslationUnit(
          emit->translation_unit_name, *(emit->unsaved_files), *(emit->flags.get()));

      Json::Value response;
      response[0] = 0;
      response[1]["cnt"] = emit->parse_id;
      response[1]["filename"] = emit->translation_unit_name;
      response[1]["bufnr"] = emit->buf_num;
      for (const auto &highlight : highlights) {
        Json::Value value;
        value["text"] = highlight.text_;
        value["type"] = highlight.type_;
        value["line"] = highlight.line_;
        value["col"] = highlight.col_;
        response[1]["highlights"].append(value);
      }

      std::ostringstream oss;
      writer_->write(response, &oss);
      std::string rsp_str = oss.str();

      stdout_lock_.lock();
      write(STDOUT_FILENO, rsp_str.c_str(), rsp_str.size());
      stdout_lock_.unlock();

      LOG(INFO)<<"Parsing finish! Id:"<<emit->parse_id<<" ,Bufnr:"<<emit->buf_num<<" ,File:"<<emit->translation_unit_name;

      // decrease the thread_number_ so other request can by emit
      thread_counter_lock_.lock();
      thread_number_--;
      LOG(INFO)<<"Thread number--:"<<thread_number_;
      thread_counter_lock_.unlock();
    }, req);
  }else{
    LOG(INFO)<<"Not enough thread, request dropped!!!";
  }
}
