var x = function(a) {
  var c = 0;

  var b = function(z) {
    c = add(z, c);
  };

  return b;
};

var q = { x: x };

var x = get(q, "x");

var b = x(1);

var _ = print(b);