#!/bin/bash -xe

for f in ./build/flang ./build/flang_frontend_tester; do
  if [ -f $f ]; then
    rm $f
  fi
done