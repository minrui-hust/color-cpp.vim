#! /bin/bash

rm -rf ./server/server
rm -rf ./server/build
mkdir -p ./server/build
cd ./server/build
cmake ..
make

