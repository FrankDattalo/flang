from __future__ import print_function

def fib(iters, prev, curr):
  if iters > 1:
    return fib(iters - 1, curr, curr + prev)
  else:
    return curr

iterations = 2

print(1)
print(1)

while iterations < 50:
  print(fib(iterations, 1, 1))
  iterations = iterations + 1
