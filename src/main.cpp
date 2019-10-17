#include "lib.hpp"

#include "Reader.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"

int main() {
  std::string data =
  R"(var a = function(hello, world) {
      if (greater(hello, world)) {
        for (var i = 0; less(i, 100); i = add(i, 1)) {
          var b00 = false;
          var xOO = undefined;
          var z_ = true;
          var _q = "this is a test";
          _q = 5.4;
          break;
        }
      } else {
        return world[hello.x];
        var n = {
          b: 1,
          c: function(q) {
            return 1;
          },
          d: 3.5,
          z: 0.15,
          f: 0
        };
      }
    };
    println(a(2, 1));
    println(a({ x: 1 }, [ "apple", "sauce" ]));
    )";

  auto reader = std::make_shared<Reader>(data);

  auto tokenizer = std::make_shared<Tokenizer>(reader);

  TokenType t = TokenType::EndOfFile;

  std::cout << "Tokens:" << std::endl;

  do {
    auto token = tokenizer->nextToken();
    std::cout << token->toString() << std::endl;
    t = token->tokenType;

    if (t == TokenType::Unknown) {
      std::cout << "UNKNOWN TOKEN!!!!: " << token->toString() << std::endl;
    }

  } while (t != TokenType::EndOfFile);

  std::cout << "End Tokens" << std::endl;

  return 0;
}