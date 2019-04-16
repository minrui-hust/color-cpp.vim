#! /bin/bash

rm -rf ./server/server
rm -rf ./server/build
mkdir -p ./server/build
cd ./server/build

ln -sf `pwd`/../../../YouCompleteMe/third_party/ycmd/third_party/clang/lib/libclang.so.7 libclang.so

cmake ..
make
