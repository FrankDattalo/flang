#!/bin/bash -xe

./scripts/clean.sh

./scripts/build.sh

./build/flang $@