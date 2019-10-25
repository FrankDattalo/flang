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

  run("var a = function(hello, world) {};");
  run("var a = 1;");
  run("if (true) {} else {}");
  run("if(true){} else {}");
  run("if(true){}else{}");
  run("if (true) {}else{}");
  run("while (true) {}");
  run("while(true){}");
  run("break ;");
  run("break;");
  run("{ break; }");
  run(
    R"(
      return;
      break;
      {
        break;
        return;
        {
          break;
          return;
        }
      }
      var x = function() {
        break;
        return;
      };
      var b = function() {
        {
          break;
          {
            {
              {
                break;
                while(true) {
                  break;
                }
              }
            }
          }
        }
        return;
      };
    )"
  );

  run("return;");
  run("return ;");
  run("return 1 ;");
  run("return 2;");
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

      var _ = println(b);
    )"
  );
  run(
    R"(
      var x = a (1);
      var y = z                 (1);
    )"
  );



  return 0;
}