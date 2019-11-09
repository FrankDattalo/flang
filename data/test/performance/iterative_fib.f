var current = 1;
var previous = 1;
var iterations = 2;

print(previous);
print("\n");

print(current);
print("\n");

while(less(iterations, 50)) {
  var tmp = current;
  current = add(current, previous);
  previous = tmp;
  iterations = add(iterations, 1);
  print(current);
  print("\n");
}