var println = function(x) {
  print(x);
  print("\n");
};

var fib = function(iters, prev, curr) {
  if (lessOrEqual(iters, 2)) {
    return curr;
  } else {
    return fib(subtract(iters, 1), curr, add(prev, curr));
  }
};

var iters = 50;

if (iters) {
  println(fib(iters, 1, 1));
} else {
  println("Bad input!");
}

var a = function() {
  println("Hello from A");
  var b = function() {
    return function() {
      return b;
    };
  };
  return b;
};

var x = a();

println(x);

var y = x();

println(y);
println(equal(x, y));

var z = y();
println(z);
println(equal(x, z));