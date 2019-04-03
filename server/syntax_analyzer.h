#pragma once
#include <mutex>
#include <memory>
#include "json/json.h"
#include "ClangAnalyzer/ClangAnalyzer.h"

class SyntaxAnalyzer{
  public:
    SyntaxAnalyzer();
    void processRequest(const Json::Value& request);

  private:
    std::mutex stdout_lock_;
    std::unique_ptr<Json::StreamWriter> writer_;
    std::unique_ptr<ClangAnalyzer> clang_analyzer_;
};
