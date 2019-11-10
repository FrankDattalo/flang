#!/bin/bash -xe

buildType=$1

case "$buildType" in
Debug)
  ;;
Release)
  ;;
*)
  echo Unknown build type: \""$buildType"\", valid types: Debug,Release
  exit 1
esac

cd ./build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=$buildType -G Ninja ..
ninja -v