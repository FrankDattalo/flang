#include "SemanticAnalyzer.hpp"

class Scope {
private:
  const std::optional<std::shared_ptr<Scope>> outerScope;
  std::map<std::string, std::shared_ptr<Token>> localScope;

  explicit Scope() noexcept
  : outerScope{std::nullopt}
  {}

  explicit Scope(std::shared_ptr<Scope> outerScope) noexcept
  : outerScope{outerScope}
  {}

  ~Scope() = default;

  bool hasOuterScope() const {
    return this->outerScope.has_value();
  }

  void define(const std::string & variableName, std::shared_ptr<Token> token) {
    localScope.insert(std::make_pair(variableName, token));
  }

  bool isDefinedLocally(const std::string & variableName) {
    return this->localScope.find(variableName) != this->localScope.end();
  }

  bool isDefined(const std::string & variableName) {

    Scope* scopeToSearch = this;

    while (true) {

      if (scopeToSearch->isDefinedLocally(variableName)) {
        return true;
      }

      if (scopeToSearch->outerScope) {
        scopeToSearch = outerScope.value().get();

      } else {
        return false;
      }
    }
  }
};

bool SemanticAnalyzer::isValid(const std::shared_ptr<ScriptAstNode>& script) noexcept {
  bool result = true;

  if (!this->allVariablesDefinedBeforeUse(script)) {
    result = false;
  }

  if (!this->allVariablesDeclaredOnlyOnceWithinScope(script)) {
    result = false;
  }

  if (!this->breaksOnlyInLoops(script)) {
    result = false;
  }

  if (!this->returnsOnlyInFunctions(script)) {
    result = false;
  }

  return result;
}

bool SemanticAnalyzer::allVariablesDefinedBeforeUse(const std::shared_ptr<ScriptAstNode>&  /*script*/) noexcept {
  return true;
}

bool SemanticAnalyzer::returnsOnlyInFunctions(const std::shared_ptr<ScriptAstNode>& script) noexcept {

  std::list<std::shared_ptr<StatementAstNode>> statementsToExplore;

  for (auto const& statement : script->statements) {
    statementsToExplore.push_back(statement);
  }

  bool noError = true;

  while (!statementsToExplore.empty()) {
    const auto statement = statementsToExplore.front();

    statementsToExplore.pop_front();

    const auto statementPointer = statement.get();

    const auto returnStatementPointer = dynamic_cast<ReturnStatementAstNode*>(statementPointer);
    // things which we will search
    const auto blockStatementPointer = dynamic_cast<BlockStatementAstNode*>(statementPointer);

    if (returnStatementPointer != nullptr) {
      this->reportError(returnStatementPointer->returnToken,
        "Return statements may only be used from within function bodies.");

      noError = false;

    } else if (blockStatementPointer != nullptr) {
      for (auto const& statement : blockStatementPointer->statements) {
        statementsToExplore.push_back(statement);
      }
    }
  }

  return noError;
}

bool SemanticAnalyzer::breaksOnlyInLoops(const std::shared_ptr<ScriptAstNode>& script) noexcept {

  std::list<std::shared_ptr<StatementAstNode>> statementsToExplore;

  for (auto const& statement : script->statements) {
    statementsToExplore.push_back(statement);
  }

  bool noError = true;

  while (!statementsToExplore.empty()) {
    const auto statement = statementsToExplore.front();

    statementsToExplore.pop_front();

    const auto statementPointer = statement.get();
    // check if this is an error
    const auto breakStatementPointer = dynamic_cast<BreakStatementAstNode*>(statementPointer);
    // things which we will search
    const auto blockStatementPointer = dynamic_cast<BlockStatementAstNode*>(statementPointer);
    const auto declareStatementPointer = dynamic_cast<DeclareStatementAstNode*>(statementPointer);
    const auto assignStatementPointer = dynamic_cast<AssignStatementAstNode*>(statementPointer);

    if (breakStatementPointer != nullptr) {
      this->reportError(breakStatementPointer->breakToken,
        "Break statements may only be used from within loops.");
      noError = false;

    } else if (blockStatementPointer != nullptr) {
      for (auto const& statement : blockStatementPointer->statements) {
        statementsToExplore.push_back(statement);
      }
    } else if (declareStatementPointer != nullptr || assignStatementPointer != nullptr) {
      FunctionDeclarationExpressionAstNode* fn = nullptr;

      if (declareStatementPointer != nullptr) {
        fn = dynamic_cast<FunctionDeclarationExpressionAstNode*>(declareStatementPointer->expression.get());
      } else {
        fn = dynamic_cast<FunctionDeclarationExpressionAstNode*>(assignStatementPointer->expression.get());
      }

      if (fn != nullptr) {
        for (auto const& statement : fn->statements) {
          statementsToExplore.push_back(statement);
        }
      }
    }
  }

  return noError;
}

bool SemanticAnalyzer::allVariablesDeclaredOnlyOnceWithinScope(const std::shared_ptr<ScriptAstNode>&  /*script*/) noexcept {
  return true;
}

void SemanticAnalyzer::reportError(
      const std::shared_ptr<Token>& token,
      const std::string & errorMessage) noexcept {

  ErrorUtils::reportErrorAtToken(
    this->out, "semantic analysis", this->reader, token, errorMessage);
}