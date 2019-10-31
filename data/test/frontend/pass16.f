var main = function(y) {
  var x = 1;
  var z = y(x);
  {
    z = add(z, 1);
  }
  return z;
};

var _ = main();