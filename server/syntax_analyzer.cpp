#include "syntax_analyzer.h"
#include "glog/logging.h"
#include <future>
#include <iostream>
#include <unistd.h>

SyntaxAnalyzer::SyntaxAnalyzer() {
  Json::StreamWriterBuilder builer;
  writer_ = std::unique_ptr<Json::StreamWriter>(builer.newStreamWriter());
  clang_analyzer_ = std::make_unique<ClangAnalyzer>();
}

void SyntaxAnalyzer::processRequest(const Json::Value &request) {
  std::string translation_unit_name = request[1]["filename"].asString();
  int parse_cnt = request[1]["cnt"].asInt();
  int buffer_number = request[1]["bufnr"].asInt();

  std::cerr << "Parsing: " << translation_unit_name << std::endl;
  std::cerr << "cnt: " << parse_cnt << std::endl;

  UnsavedFile unsaved;
  unsaved.filename_ = request[1]["unsaved"]["filename"].asString();
  unsaved.contents_ = request[1]["unsaved"]["content"].asString();
  unsaved.length_ = unsaved.contents_.size();
  std::vector<UnsavedFile> unsaved_files{unsaved};

  std::vector<std::string> flags;
  for (auto const &flag : request[1]["flags"]) {
    flags.emplace_back(flag.asString());
  }

  std::async(std::launch::async, [&]() {
    auto highlights = clang_analyzer_->UpdateTranslationUnit(
        translation_unit_name, unsaved_files, flags);

    // LOG(INFO)<<"Total highlight: "<<highlights.size();

    Json::Value response;
    response[0] = 0;
    response[1]["cnt"] = parse_cnt;
    response[1]["filename"] = translation_unit_name;
    response[1]["bufnr"] = buffer_number;
    for (const auto &highlight : highlights) {
      Json::Value value;
      value["text"] = highlight.text_;
      value["type"] = highlight.type_;
      value["line"] = highlight.line_;
      value["col"] = highlight.col_;
      response[1]["highlights"].append(value);
      // LOG(INFO)<<highlight.text_<<" "<<highlight.type_<<"
      // "<<highlight.line_<<" "<<highlight.col_;
    }

    std::ostringstream oss;
    writer_->write(response, &oss);
    std::string rsp_str = oss.str();

    // LOG(INFO)<<"\n"<<rsp_str;

    stdout_lock_.lock();
    write(STDOUT_FILENO, rsp_str.c_str(), rsp_str.size());
    stdout_lock_.unlock();

    std::cerr << "Parse Done: " << translation_unit_name << std::endl;
    std::cerr << "cnt: " << parse_cnt << std::endl;
  });
}
