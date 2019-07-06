#pragma once
#include <mutex>
#include <memory>
#include "json/json.h"
#include "ClangAnalyzer/ClangAnalyzer.h"

class SyntaxAnalyzer{
  public:
    struct Request{
      int parse_id;
      int buf_num;
      std::string translation_unit_name;
      std::shared_ptr<std::vector<UnsavedFile>> unsaved_files;
      std::shared_ptr<std::vector<std::string>> flags;
    };

    SyntaxAnalyzer();
    void processRequest(const Json::Value& request);

  private:
    static const int kMaxThreadNum;
    int thread_number_ = 0;
    std::mutex thread_counter_lock_;
    std::mutex stdout_lock_;
    std::unique_ptr<Json::StreamWriter> writer_;
    std::unique_ptr<ClangAnalyzer> clang_analyzer_;
};
