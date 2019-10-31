#!/bin/bash -xe

python \
  ./scripts/run-clang-tidy.py \
  -p ./build \
  -header-filter=include \
  -checks=cppcoreguidelines-*,misc-*,clang-analyzer-*,modernize-*,performance-*,readability-*,-cppcoreguidelines-pro-bounds-constant-array-index,-cppcoreguidelines-special-member-functions
