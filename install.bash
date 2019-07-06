#! /bin/bash

rm -rf ./server/build
mkdir -p ./server/build
cd ./server/build
cmake .. -DJSONCPP_WITH_TESTS=OFF -DCMAKE_EXPORT_COMPILE_COMMANDS=yes
make
