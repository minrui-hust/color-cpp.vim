cmake_minimum_required(VERSION 2.8.3)

project(colorcpp_server)

set(CMAKE_CXX_FLAGS "-std=c++14 -march=native -pipe -O2 -Wall -Wextra -fopenmp -fPIC -pthread")

include_directories(
  third_party/jsoncpp-1.8.4/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../../YouCompleteMe/third_party/ycmd/cpp/llvm/include
  )

link_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../YouCompleteMe/third_party/ycmd/third_party/clang/lib
  )

add_executable(clang_server 
               server.cpp
               json_parser.cpp
               syntax_analyzer.cpp
               )
target_link_libraries(clang_server
  jsoncpp
  glog
  clang_analyzer
  clang
  )

add_custom_command(
    TARGET clang_server
    POST_BUILD
    COMMAND cp ARGS ${CMAKE_CURRENT_BINARY_DIR}/clang_server ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(ClangAnalyzer)
add_subdirectory(third_party/jsoncpp-1.8.4)
