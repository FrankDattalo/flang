#include "lib.hpp"

#include "StringReader.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"

void run(const std::string & str) {
  auto reader = std::make_shared<StringReader>(str);

  auto tokenizer = std::make_shared<Tokenizer>(reader);

  auto parser = std::make_shared<Parser>(tokenizer, std::cout);

  std::cout << "begin parse of '" << str << "'" << std::endl;

  auto script = parser->parseScript();

  if (script) {
    std::cout << "parsed ok" << std::endl;
  } else {
    std::cout << "failed to parse" << std::endl;
  }
}

int main(int  /*argc*/, char*  /*argv*/[]) {

  // std::cout << "Arg count: " << argc << std::endl;
  // for (int i = 0; i < argc; i++) {
  //   std::cout << "Arg " << i << ") " << argv[i] << std::endl;
  // }

  run(R"(var a = function(hello, world) {};)");
  run("var a = 1;");
  run("if (true) {} else {}");
  run("if(true){} else {}");
  run("if(true){}else{}");
  run("if (true) {}else{}");
  run("while (true) {}");
  run("while(true){}");
  run("break ;");
  run("break;");
  run("return;");
  run("return ;");
  run("return 1 ;");
  run("return 2;");
  run("var x = function(a) { return add(1, a); };");
  run("var x = function(a) { return { b: 1, a: a }; };");
  run("var x = function(a) { return { b: 1, a: 2 }; };");
  run("var x = function(a) { return { b: 1 }; };");
  run("var x = function(a) { return { }; };");




  return 0;
}