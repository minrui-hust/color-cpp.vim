#pragma once
#include <memory>
#include "json/json.h"

class JsonParser {
 public:
  JsonParser();
  std::shared_ptr<Json::Value> parse(const char* data, int size);

 private:
  std::unique_ptr<Json::CharReader> reader_;
  std::unique_ptr<Json::StreamWriter> writer_;
};
