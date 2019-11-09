var fib = function(fib, iters, prev, curr) {
  if (greater(iters, 1)) {
    return fib(fib, subtract(iters, 1), curr, add(prev, curr));
  } else {
    return curr;
  }
};

fib = function(iters, prev, curr) {
  return fib(fib, iters, prev, curr);
};

var iterations = 2;

print(1);
print("\n");

print(1);
print("\n");

while(less(iterations, 50)) {
  print(fib(iterations, 1, 1));
  print("\n");
  iterations = add(iterations, 1);
}