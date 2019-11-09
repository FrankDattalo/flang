var x = 1;

var z = function() {
  var z = 2;
  var x = add(x, z);
  print(x);
  print("\n");
};

z();

print(x);
print("\n");