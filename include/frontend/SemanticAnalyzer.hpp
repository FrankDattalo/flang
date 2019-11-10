#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include "utils/lib.hpp"
#include "utils/error.hpp"
#include "frontend/Ast.hpp"
#include "frontend/AstWalker.hpp"

class SemanticAnalyzer {
private:
  std::ostream & out;

public:
  explicit SemanticAnalyzer(std::ostream & out) noexcept;

  bool IsValid(const ScriptAstNode& script) noexcept;
};

#endif // SEMANTIC_ANALYZER_HPP