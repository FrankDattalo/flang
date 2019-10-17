#!/bin/bash -xe

if [ -f ./build/flang ]; then 
  rm ./build/flang 
fi

./build.sh

./build/flang