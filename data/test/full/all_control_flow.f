

var _ = undefined;

_ = print("If statement without else\n");

if (true) {
  _ = print("This should be printed\n");
}

_ = print("Step 2: If statements with else\n");

if (false) {
  _ = print("This should never be printed\n");
} else {
  _ = print("This should be printed\n");
}

if (true) {
  _ = print("This should be printed\n");
} else {
  _ = print("This should never be printed\n");
}

_ = print("while without break\n");

var x = 1;

_ = print("This should print 1 - 9\n");

while (less(x, 10)) {
  _ = print(x); _ = print("\n");
  x = add(1, x);
}

_ = print("while with break\n");

x = 1;

_ = print("This should print 1\n");

while (less(x, 10)) {
  _ = print(x); _ = print("\n");
  x = add(1, x);
  break;
}

var fn = function(z) {
  var _ = print(z); _ = print("\n");
  return z;
};

var fn2 = function(z) {
  var _ = print(z); _ = print("\n");
};

_ = print("True should be printed twice:\n");

_ = print(fn(true)); _ = print("\n");

_ = print("false then undefined should be printed:\n");

_ = print(fn2(false)); _ = print("\n");

