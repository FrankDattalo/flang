var _ = undefined;

var x = function(a) {
  var c = 0;

  var b = function(z) {
    var c = add(z, c);
  };

  return b;
};

var q = { x: x };

var y = get(q, "x");

var b = x(1);

_ = print(b);