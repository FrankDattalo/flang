#!/home/frank/Documents/flang/build/flang

var _ = undefined;

var println = function(arg) {
  var _ = print(arg);
  _ = print("\n");
};

var i = 1;

var printPath = function(println) {
  var path = env("PATH");
  var _ = print("Path: ");
  _ = println(path);
  return;
};

var evokeFn = function(f, arg1, arg2, arg3) {
  return f(arg1, arg2, arg3);
};

var add3 = function(a1, a2, a3) { return add(a1, add(a2, a3)); };

_ = println("evokeFn(add3, int(read()), int(read()), int(read())):");
_ = println(evokeFn(add3, int(read()), int(read()), int(read())));

while (lessOrEqual(i, 10)) {
  _ = println(append(append("Execution ", i), " of 10"));
  _ = print("Enter your name: ");

  var name = read();

  _ = println(append("Your name is: ", name));

  if (equal(name, "quit")) {
    _ = println("Name was quit!");

  } else {
    _ = println("Name was not quit!");
  }

  if (equal(name, "quit")) {
    _ = printPath(println);
    break;
  }

  i = add(i, 1);
}