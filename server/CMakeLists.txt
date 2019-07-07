cmake_minimum_required(VERSION 2.8.3)

project(colorcpp_server)

option(LOG_ENABLE "Whether enable the server log" OFF)

set(CMAKE_CXX_DEFINES)

if(${LOG_ENABLE} STREQUAL "ON")
  list(APPEND CMAKE_CXX_DEFINES -DLOG_ENABLE)
endif()

set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_DEFINES} -march=native -pipe -O2 -Wall -Wextra -Wno-unused-result -fopenmp -fPIC -pthread")

include_directories(
  third_party/jsoncpp-1.8.4/include
  third_party/clang/llvm/include
  )

link_directories(
  third_party/clang/lib
  )

add_executable(clang_server 
               server.cpp
               json_parser.cpp
               syntax_analyzer.cpp
               )
target_link_libraries(clang_server
  jsoncpp
  clang_analyzer
  clang
  )

add_subdirectory(ClangAnalyzer)
add_subdirectory(third_party/jsoncpp-1.8.4)
