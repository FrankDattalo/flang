var println = function(x) {
  print(x);
  print("\n");
};

var tag = function(name) {
  return function(inner) {
    print("<");
    print(name);
    print(">");

    if (equal(type(inner), "string")) {
      print(inner);
    } else {
      inner();
    }

    print("</");
    print(name);
    print(">");
  };
};

var html = tag("html");
var body = tag("body");
var h1 = tag("h1");

html(function() {
  body(function() {
    h1("Hello from Flang w/ Closures");
  });
});