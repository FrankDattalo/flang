#!/bin/bash -xe

./scripts/clean.sh

./scripts/clang-tidy.sh

./scripts/build.sh

./build/flang $@