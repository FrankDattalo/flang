#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "lib.hpp"
#include "Readable.hpp"
#include "StringReader.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"
#include "TokenBuffer.hpp"
#include "Parser.hpp"
#include "SemanticAnalyzer.hpp"
#include "AstCompiler.hpp"
#include "Runtime.hpp"

namespace interpreter {

class Interpreter {
private:
  std::ostream & out;
  std::istream & in;

public:

  explicit Interpreter(std::ostream & out, std::istream & in) noexcept
  : out{out}
  , in{in}
  {}

  void Run(const std::string & data);
};

}

#endif
