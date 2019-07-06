#! /bin/bash

CMAKE_OPTION=""
if [ -n "$1" ]; then
  CMAKE_OPTION="-DLOG_ENABLE=ON"
fi

rm -rf ./server/build
mkdir -p ./server/build
cd ./server/build
cmake .. ${CMAKE_OPTION} -DJSONCPP_WITH_TESTS=OFF -DCMAKE_EXPORT_COMPILE_COMMANDS=yes
make
