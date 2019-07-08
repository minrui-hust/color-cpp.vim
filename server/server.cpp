#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <memory>

#include "glog/logging.h"
#include "json/json.h"

#include "json_parser.h"
#include "syntax_analyzer.h"

constexpr int kStdinBufferSize = 4 * 1024 * 1024;  // 4M

void setupLog(char** argv) {
  // get the path of server
  std::string this_program_path(argv[0]);
  int pos = this_program_path.find_last_of('/');
  if (pos < 0) {
    return;
  }
  std::string this_program_folder = this_program_path.substr(0, pos);

  // check if log folder exist
  if (access((this_program_folder + "/../log").c_str(), 0) < 0) {
    mkdir((this_program_folder + "/../log").c_str(), 0777);
  }

  std::freopen((this_program_folder + "/../log/stderr.log").c_str(), "w", stderr);
}

int main(int /* argc */, char** argv) {
#ifdef LOG_ENABLE
  setupLog(argv);
#endif

  char* stdin_buf = new char[kStdinBufferSize + 1];
  size_t stdin_read_size;
  JsonParser parser;
  SyntaxAnalyzer analyzer;
  std::shared_ptr<std::string> response;

  while (true) {
    stdin_read_size = read(STDIN_FILENO, stdin_buf, kStdinBufferSize);
    if (stdin_read_size > 0) {
      auto json_value = parser.parse(stdin_buf, stdin_read_size);
      if (json_value) {
        analyzer.processRequest(*json_value);
      } else {
        std::cerr << "Parse json packet failed";
      }
    }
  }

  return 0;
}
