from __future__ import print_function

current = 1
previous = 1
iterations = 2

print(previous)
print(current)

while iterations < 50:
  tmp = current
  current = current + previous
  previous = tmp
  iterations = iterations + 1
  print(current)
