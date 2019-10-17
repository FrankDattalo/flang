#!/bin/bash -xe

cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja ..
python run-clang-tidy.py -fix -style=Google -header-filter=include -checks=cppcoreguidelines-*,misc-*,clang-analyzer-*,modernize-*,performance-*,readability-*,-cppcoreguidelines-pro-bounds-constant-array-index,-cppcoreguidelines-special-member-functions
ninja