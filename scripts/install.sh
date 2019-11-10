#!/bin/bash -xe

./scripts/build.sh Release

sudo cp ./build/flang /usr/bin