#include "lib.hpp"

#include "StringReader.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"
#include "TokenBuffer.hpp"
#include "Parser.hpp"
#include "SemanticAnalyzer.hpp"

void run(const std::string & str) {
  auto reader = std::make_shared<StringReader>(str);
  auto tokenizer = std::make_shared<Tokenizer>(reader);
  auto tokenBuffer = std::make_shared<TokenBuffer>(tokenizer);
  auto parser = std::make_shared<Parser>(tokenBuffer, std::cout);

  std::cout << "begin parse of '" << str << "'" << std::endl;

  auto script = parser->parseScript();

  if (script) {
    std::cout << "parsed ok" << std::endl;
  } else {
    std::cout << "failed to parse" << std::endl;
    return;
  }

  auto analyzer = std::make_shared<SemanticAnalyzer>(std::cout, reader);

  if (!analyzer->isValid(script.value())) {
    std::cout << "semantic analysis failed" << std::endl;
  } else {
    std::cout << "semantic analysis ok" << std::endl;
  }
}

int main(int  /*argc*/, char*  /*argv*/[]) {

  // std::cout << "Arg count: " << argc << std::endl;
  // for (int i = 0; i < argc; i++) {
  //   std::cout << "Arg " << i << ") " << argv[i] << std::endl;
  // }

  run("var x = function(a) { return add(1, a); };");
  run("var x = function(a) { return { b: 1, a: a }; };");
  run("var x = function(a) { return { b: 1, a: 2 }; };");
  run("var x = function(a) { return { b: 1 }; };");
  run(
    R"(
      var x = {
        y: function() {
          break;
        },
        z: {
          b: function() {
            break;
            if (true) {
              break;
            } else {
              break;
            }
          }
        }
      };
    )"
  );
  run("var x = function(a) { return { }; };");
  run(
    R"(
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
    )"
  );
  run(
    R"(
      var x = a (1);
      var y = z                 (1);
    )"
  );
  run(
    R"(
      {
        break;
      }
      while (true) {
        return;
      }
    )"
  );
  run(R"(
    var x = 100000000000000000000000000000000000000000000000000000000000000000000000;
    var y = 10000000000000000000000000000000000000000000000.00000000000000000000000000000000000000000000000000000000000000000000001;
  )");
  run(
    R"(
      var add = 1;
      var get = 1;
      var print = 1;

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

      var _ = print(b);
    )"
  );

  run(
    R"(
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
    )"
  );
  run(R"(
    var _ = add(
      function() {
        if (greater(1, 2)) {
          var _ = print("Greater!");
        }
        break;
      }
    );
  )");
  run(R"(
    var x = 1;
    var _ = function() {
      var y = x();
      y = add(x, 1);
    };
  )");
  run(R"(
    var _ = function() {
      var x = undefined;
      if (true) {
        x = 1;
      } else {
        x = 2;
      }
    };
  )");
  run(R"(
    var main = function(y) {
      var x = 1;
      var z = y(x);
      {
        z = add(z, 1);
      }
      return z;
    };

    var _ = main();
  )");



  return 0;
}