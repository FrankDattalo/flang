#!/bin/bash -xe

cd ./build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja ..
ninja