
#
# Hello!
# This is some fLang source code with syntax highlighting!
#

var tag = function(name) {
  return function(inner) {
    print("<");
    print(name);
    print(">");

    var innerType = type(inner);

    if (equal(innerType, "string")) {
      print(inner);

    } else if (equal(innerType, "function")) {
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
var head = tag("head");
var title = tag("title");

html(function() {
  head(function() {
    title("This is my title", "Test");
  });
  body(function() {
    h1("Hello from Flang w/ Closures");
  });
});