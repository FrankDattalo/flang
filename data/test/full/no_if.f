
var greet = function(name) {
  var _ = print(append("Hello, ", name));
  return true;
};

var i = 0;
var max = 10;

while (less(i, max)) {
  var j = 0;

  while (less(j, max)) {
    var _ = print(greet(j));
    j = add(j, 1);
  }

  i = add(i, 1);
}

