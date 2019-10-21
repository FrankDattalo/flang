#!/bin/bash -xe

cd ./build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja ..
cd ..
#python run-clang-tidy.py -style=Google -header-filter=include -checks=cppcoreguidelines-*,misc-*,clang-analyzer-*,modernize-*,performance-*,readability-*,-cppcoreguidelines-pro-bounds-constant-array-index,-cppcoreguidelines-special-member-functions


clang-tidy  $(find src/ -name *.cpp) -p ./build/ -header-filter=include -checks=cppcoreguidelines-*,misc-*,clang-analyzer-*,modernize-*,performance-*,readability-*,-cppcoreguidelines-pro-bounds-constant-array-index,-cppcoreguidelines-special-member-functions


ninja