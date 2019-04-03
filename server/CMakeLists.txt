cmake_minimum_required(VERSION 2.8.3)

project(colorcpp_server)

set(CMAKE_CXX_FLAGS "-std=c++14 -march=native -pipe -O2 -Wall -Wextra -fopenmp -fPIC -pthread")

set(LIBCLANG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../YouCompleteMe/third_party/ycmd/third_party/clang/lib)

find_package(jsoncpp REQUIRED)

include_directories(
  /usr/local/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../../YouCompleteMe/third_party/ycmd/cpp/llvm/include
  )

link_directories(
  /usr/local/lib
  ${LIBCLANG_DIR}
  )

add_executable(server 
               server.cpp
               json_parser.cpp
               syntax_analyzer.cpp
               )
target_link_libraries(server
  jsoncpp
  glog
  clang_analyzer
  clang
  )

add_custom_command(
    TARGET server
    POST_BUILD
    COMMAND cp ARGS ${CMAKE_CURRENT_BINARY_DIR}/server ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(ClangAnalyzer)
