var recurse = function(f) {
  var baseFn = f();
  var boundBaseFn = f(baseFn);
};

var fib = recurse(function(fib, iters, prev, curr) {
  if (greater(iters, 1)) {
    return fib(subtract(iters, 1), curr, add(prev, curr));
  } else {
    return curr;
  }
});