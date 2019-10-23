#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include "lib.hpp"
#include "Ast.hpp"
#include "Error.hpp"
#include "Readable.hpp"

class SemanticAnalyzer {
private:
  std::ostream & out;
  const std::shared_ptr<Readable> reader;

public:
  explicit SemanticAnalyzer(std::ostream & out, std::shared_ptr<Readable> reader) noexcept
  : out{out}, reader{std::move(reader)}
  {}

  virtual ~SemanticAnalyzer() = default;

  bool isValid(const std::shared_ptr<ScriptAstNode>& script) noexcept;

private:

  bool allVariablesDefinedBeforeUse(const std::shared_ptr<ScriptAstNode>& script) noexcept;

  bool returnsOnlyInFunctions(const std::shared_ptr<ScriptAstNode>& script) noexcept;

  bool breaksOnlyInLoops(const std::shared_ptr<ScriptAstNode>& script) noexcept;

  bool allVariablesDeclaredOnlyOnceWithinScope(const std::shared_ptr<ScriptAstNode>& script) noexcept;

  void reportError(
    const std::shared_ptr<Token>& token,
    const std::string & errorMessage) noexcept;
};

#endif // SEMANTIC_ANALYZER_HPP