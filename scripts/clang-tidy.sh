#!/bin/bash -xe

cd build
python run-clang-tidy.py -header-filter=include -checks=cppcoreguidelines-*,misc-*,clang-analyzer-*,modernize-*,performance-*,readability-*,-cppcoreguidelines-pro-bounds-constant-array-index,-cppcoreguidelines-special-member-functions
