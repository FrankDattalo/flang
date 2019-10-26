#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include "lib.hpp"
#include "Ast.hpp"
#include "Error.hpp"
#include "Readable.hpp"
#include "AstWalker.hpp"

class Scope {
private:
  const std::optional<std::shared_ptr<Scope>> outerScope;
  std::map<std::string, std::shared_ptr<Token>> localScope;

public:
  explicit Scope() noexcept
  : outerScope{std::nullopt}
  {}

  explicit Scope(std::shared_ptr<Scope> outerScope) noexcept
  : outerScope{outerScope}
  {}

  ~Scope() = default;

  bool hasOuterScope() const;

  void define(const std::string & variableName, std::shared_ptr<Token> token);

  std::optional<std::shared_ptr<Token>> findLocally(const std::string & variableName);

  std::optional<std::shared_ptr<Token>> find(const std::string & variableName);

  std::optional<std::shared_ptr<Scope>> getOuterScope();
};

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

  // bool allVariablesDefinedBeforeUse(const std::shared_ptr<ScriptAstNode>& script) noexcept;

  // bool returnsOnlyInFunctions(const std::shared_ptr<ScriptAstNode>& script) noexcept;

  // bool breaksOnlyInLoops(const std::shared_ptr<ScriptAstNode>& script) noexcept;

  // bool allVariablesDeclaredOnlyOnceWithinScope(const std::shared_ptr<ScriptAstNode>& script) noexcept;

  void reportError(
    const std::shared_ptr<Token>& token,
    const std::string & errorMessage) noexcept;
};

class SemanticAnalyzerRun : public AstWalker {
private:
  SemanticAnalyzer* semanticAnalyzer;
  bool error;
  std::size_t functionDept;
  std::vector<bool> inLoop;
  std::shared_ptr<Scope> currentScope;

public:
  explicit SemanticAnalyzerRun(SemanticAnalyzer*  semanticAnalyzer) noexcept
  : semanticAnalyzer{semanticAnalyzer}
  , error{false}
  , functionDept{0}
  , inLoop{{false}}
  , currentScope{std::make_shared<Scope>()}
  {}

  ~SemanticAnalyzerRun() noexcept override = default;

  bool hasErrors() noexcept {
    return this->error;
  }

  // validate that the variable we are delcaring has not already been declared elsewhere
  void onEnterDeclareStatementAstNode(DeclareStatementAstNode* node) noexcept override;

  // validate that the variable we are assign has been declared within the current scope
  void onEnterAssignStatementAstNode(AssignStatementAstNode* node) noexcept override;

  // validate that we're in a loop
  void onEnterBreakStatementAstNode(BreakStatementAstNode* node) noexcept override;

  // validate that function dept is not zero
  void onEnterReturnStatementAstNode(ReturnStatementAstNode* node) noexcept override;

  // increase scope
  void onEnterBlockStatementAstNode(BlockStatementAstNode* node) noexcept override;

  // mark in loop to true
  void onEnterWhileStatementAstNode(WhileStatementAstNode*  node) noexcept override;

  // validate that identifier is defined
  void onEnterIdentifierExpressionAstNode(IdentifierExpressionAstNode* node) noexcept override;

  // validate that identifier is defined
  void onEnterFunctionInvocationExpressionAstNode(FunctionInvocationExpressionAstNode* node) noexcept override;

  // increase scope, define arguments
  void onEnterFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode* node) noexcept override;

  // define the variable within the current scope
  void onExitDeclareStatementAstNode(DeclareStatementAstNode* node) noexcept override;

  // pop scope
  void onExitBlockStatementAstNode(BlockStatementAstNode* node) noexcept override;

  // mark in loop to false
  void onExitWhileStatementAstNode(WhileStatementAstNode*  node) noexcept override;

  // pop scope
  void onExitFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode* node) noexcept override;
};

#endif // SEMANTIC_ANALYZER_HPP