cmake_minimum_required(VERSION 2.8.3)

project(colorcpp_server)

option(LOG_ENABLE "Whether enable the server log" OFF)

set(CMAKE_CXX_DEFINES)

if(${LOG_ENABLE} STREQUAL "ON")
  list(APPEND CMAKE_CXX_DEFINES -DLOG_ENABLE)
endif()

set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_DEFINES} -march=native -pipe -O2 -Wall -Wextra -Wno-unused-result -fopenmp -fPIC -pthread")

# if YCM directory structure changes, modify here
set(YCM_PATH ${CMAKE_SOURCE_DIR}/../../YouCompleteMe/)
set(CLANG_INC_DIR ${YCM_PATH}/third_party/ycmd/cpp/llvm/include)
set(CLANG_LIB_DIR ${YCM_PATH}/third_party/ycmd/third_party/clang/lib)

file(GLOB CLANG_LIB_FILE "${CLANG_LIB_DIR}/libclang*" )
if(CLANG_LIB_FILE)
  message("Clang lib: ${CLANG_LIB_FILE}")
else()
  message(FATAL_ERROR "Could not find libclang")
endif()

include_directories(
  third_party/jsoncpp-1.8.4/include
  ${CLANG_INC_DIR}
  )

add_executable(clang_server 
               server.cpp
               json_parser.cpp
               syntax_analyzer.cpp
               )
target_link_libraries(clang_server
  jsoncpp
  clang_analyzer
  ${CLANG_LIB_FILE}
  )

add_subdirectory(ClangAnalyzer)
add_subdirectory(third_party/jsoncpp-1.8.4)
