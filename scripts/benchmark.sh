#!/bin/bash

function run () {
  description=$1
  command=$2
  echo $description: $command
  multitime -q -n 50 $command
}

run "Flang Iterative Fib (50)" "./build/flang ./data/test/performance/iterative_fib.f"
run "Flang Recursive Fib (50)" "./build/flang ./data/test/performance/recursive_fib.f"

run "Python3 Iterative Fib (50)" "python3 ./data/test/performance/iterative_fib.py"
run "Python3 Recursive Fib (50)" "python3 ./data/test/performance/recursive_fib.py"

run "Python2 Iterative Fib (50)" "python2 ./data/test/performance/iterative_fib.py"
run "Python2 Recursive Fib (50)" "python2 ./data/test/performance/recursive_fib.py"