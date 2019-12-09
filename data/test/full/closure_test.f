var println = function(x) {
  var _ = print(x);
  _ = print("\n");
};

var x = 1;

var y = function() {
  var _ = println(x);
};

var _ = y();

var x1 = 2;

var y1 = function() {
  return function() {
    var _ = println(x1);
    _ = println(x);
  };
};

var yy = y1();
_ = println(yy);
_ = yy();

var y2 = function() {
  var x = 3;
  return function() {
    var _ = println(x);
  };
};

var yy2 = y2();
_ = println(yy2);
_ = yy2();

var y3 = function() {
  var f = function() {
    return 4;
  };

  var q = function() {
    return function() {
      var _ = println(f());
    };
  };

  return function() { return q(); };
};

var yy3 = y3();
_ = println(yy3);
_ = yy3();
_ = println(_());

var z = function() {
  var x = 1;
  return function(y) {
    return add(y, x);
  };
};

var zz = z();
var qq = zz(1);
println(qq);


var MyObject = {
  age: "test"
};

println(set(MyObject, "age", "noodle"));
println(get(MyObject, "age"));