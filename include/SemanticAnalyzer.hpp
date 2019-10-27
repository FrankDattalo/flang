#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include "lib.hpp"
#include "Ast.hpp"
#include "Error.hpp"
#include "Readable.hpp"
#include "AstWalker.hpp"

class SemanticAnalyzer {
private:
  std::ostream & out;
  const std::shared_ptr<const Readable> reader;

public:
  explicit SemanticAnalyzer(std::ostream & out, std::shared_ptr<const Readable> reader) noexcept
  : out{out}, reader{std::move(reader)}
  {}

  virtual ~SemanticAnalyzer() = default;

  bool isValid(const std::shared_ptr<ScriptAstNode>& script) noexcept;
};

#endif // SEMANTIC_ANALYZER_HPP