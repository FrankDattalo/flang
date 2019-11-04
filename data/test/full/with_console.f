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

var evokeFn = function(println, f, arg1, arg2, arg3) {
  var _ = println(append("Evoking function: ", f));
};

while (lessOrEqual(i, 10)) {
  _ = println(append(append("Execution ", i), " of 10"));
  _ = print("Enter your name: ");

  var name = read();

  if (equal(name, "quit")) {
    _ = println("Name was quit!");
    _ = printPath(println);
    break;

  } else {
    _ = println("Name was not quit!");
  }

  i = add(i, 1);
}