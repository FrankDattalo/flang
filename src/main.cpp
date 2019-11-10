#include "utils/lib.hpp"
#include "frontend/TokenType.hpp"
#include "frontend/Token.hpp"
#include "frontend/Tokenizer.hpp"
#include "frontend/TokenBuffer.hpp"
#include "frontend/Ast.hpp"
#include "frontend/AstWalker.hpp"

// #include "Io.hpp"
// #include "Interpreter.hpp"

void usage(const std::string & msg) {
  std::cerr
    << "Error: " << msg << '\n'
    << "Usage: flang <path to source code file>" << std::endl;

  exit(1);
}

struct MyHandler {

  void onEnterIfStatementAstNode(frontend::IfStatementAstNode*) {}
  void onExitIfStatementAstNode(frontend::IfStatementAstNode*) {}


  void onEnterExpressionStatementAstNode(frontend::ExpressionStatementAstNode*) {}
  void onExitExpressionStatementAstNode(frontend::ExpressionStatementAstNode*) {}
};

int main(int, char**) {

  // if (argc != 2) {
  //   usage("Unexpected number of arguments.");
  //   return 1;
  // }

  // std::string filePath{argv[1]};

  // auto contents = io::readFileToString(filePath);

  // if (!contents) {
  //   usage("Could not open file.");
  //   return 1;
  // }

  // auto interpreter = std::make_shared<interpreter::Interpreter>(std::cout, std::cin);
  // interpreter->Run(contents.value());

  MyHandler mh;

  frontend::AstWalker<MyHandler*> walker{&mh};

  walker.VisitScriptAstNode(nullptr);

  return 0;
}