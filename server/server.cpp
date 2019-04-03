#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "json/json.h"
#include "glog/logging.h"

#include "json_parser.h"
#include "syntax_analyzer.h"

constexpr int kStdinBufferSize = 1024 * 100; // 10K

void setupGlog(char** argv){
  google::InitGoogleLogging(argv[0]);  
  google::SetLogDestination(google::GLOG_INFO, "/home/mr/.vim/bundle/color-cpp/server/log/");
}

int main(int argc, char**argv) {
  setupGlog(argv);

  char *stdin_buf = new char[kStdinBufferSize+1];
  size_t stdin_read_size;
  JsonParser parser;
  SyntaxAnalyzer analyzer;
  std::shared_ptr<std::string> response;

  while (true) {
    stdin_read_size = read(STDIN_FILENO, stdin_buf, kStdinBufferSize);
    if (stdin_read_size > 0) {
      auto json_value = parser.parse(stdin_buf, stdin_read_size);
      if(json_value){
        analyzer.processRequest(*json_value);
      }
    }
  }

  return 0;
}
