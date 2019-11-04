#!/home/frank/Documents/flang/build/flang

var _ = undefined;

var i = 1;

var printPath = function() {
  var path = env("PATH");
  var _ = print("Path: ");
  _ = print(path);
  return;
};

while (lessOrEqual(i, 10)) {
  _ = print("Execution ");
  _ = print(i);
  _ = print(" of 10");
  _ = print("\n");
  _ = print("Enter your name: ");

  var name = read();

  if (equal(name, "quit")) {
    _ = print("Name was quit!");
    _ = print("\n");
    _ = printPath();
    break;

  } else {
    _ = print("Name was not quit!");
    _ = print("\n");
  }

  i = add(i, 1);
}