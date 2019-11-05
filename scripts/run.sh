#!/bin/bash -xe

./scripts/clean.sh

./scripts/build.sh

./scripts/clang-tidy.sh

./build/flang $@