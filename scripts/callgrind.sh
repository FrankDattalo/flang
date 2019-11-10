#!/bin/bash -xe

valgrind \
  --tool=callgrind \
  ./build/flang $1