#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include "lib.hpp"
#include "Ast.hpp"

class SemanticAnalyzer {
private:
  std::ostream & out;

public:
  explicit SemanticAnalyzer(std::ostream & out) noexcept
  : out{out}
  {}

  virtual ~SemanticAnalyzer() = default;

  bool isValid(std::shared_ptr<ScriptAstNode> script) noexcept;
};

#endif // SEMANTIC_ANALYZER_HPP