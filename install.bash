#! /bin/bash

rm -rf ./server/server
rm -rf ./server/build
mkdir -p ./server/build
cd ./server/build

BUILD_DIR=`pwd`
CLANG_LIB_DIR=${BUILD_DIR}/../../../YouCompleteMe/third_party/ycmd/third_party/clang/lib

cd ${CLANG_LIB_DIR}
ln -sf libclang.so.7 libclang.so

cd ${BUILD_DIR}
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=yes
make
