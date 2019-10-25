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

bool hasScope(StatementAstNode* statement) {

  const auto ifPtr = dynamic_cast<IfStatementAstNode*>(statement);
  const auto whilePtr = dynamic_cast<WhileStatementAstNode*>(statement);
  const auto declarePtr = dynamic_cast<DeclareStatementAstNode*>(statement);
  const auto assignPtr = dynamic_cast<AssignStatementAstNode*>(statement);

  if (ifPtr != nullptr) {
    return true;
  }

  if (whilePtr != nullptr) {
    return true;
  }

  ExpressionAstNode* expression = nullptr;

  if (assignPtr != nullptr) {
    expression = assignPtr->expression.get();
  }

  if (declarePtr != nullptr) {
    expression = declarePtr->expression.get();
  }

  if (expression == nullptr) {
    return false;
  }

  const auto fn = dynamic_cast<FunctionDeclarationExpressionAstNode*>(expression);
  const auto obj = dynamic_cast<ObjectDeclarationExpressionAstNode*>(expression);

  if (fn != nullptr) {
    return true;
  }

  if (obj != nullptr) {
    return true;
  }

  return false;
}

bool allVariablesDefinedBeforeUse(
    const std::vector<std::shared_ptr<StatementAstNode>>&  /*statements*/,
    const std::shared_ptr<Scope>&  /*scope*/) {

  // when we encouter a block, define a new scope, link the outer scope, and descend there
  // after descending, pop the scope and continue looping

  // when we encouter an assign or a declare which are a function definition, link the outer scope,
  // add the parameters as defined variables within the new scope, and descend there
  // after descending, pop the scope and continue looping

  // when we encounter an if statement, check the if condition, link the outer scope, and descend there
  // after descending pop the scope, if the if statement has an else, descend there

  // when we encounter a while statement, check the condition, link the outer scope and descend there

  // when we encounter a return statement, if the condition is present, descend there

  // when we encoutner an assign or declare of an object literal, for each value, link the outer scope
  // add the parameters as defined variables in the new scope, and descend there,
  // after descending pop the scope and conitinue looping

  // when we encounter an assign or declare of a function call or identifier, validate that the
  // identifier or function identifier is defined.

  return true;
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

void exploreExpressionNonLoops(
    std::list<std::shared_ptr<StatementAstNode>>& statementsToExplore,
    ExpressionAstNode* expressionPointer) noexcept {

  const auto fn = dynamic_cast<FunctionDeclarationExpressionAstNode*>(expressionPointer);
  const auto obj = dynamic_cast<ObjectDeclarationExpressionAstNode*>(expressionPointer);

  if (fn != nullptr) {
    for (auto const& statement : fn->statements) {
      statementsToExplore.push_back(statement);
    }
  } else if (obj != nullptr) {
    for (auto const& kv : obj->keyValues) {
      auto const value = kv.second.get();
      exploreExpressionNonLoops(statementsToExplore, value);
    }
  }
}

void exploreStatementsNonLoops(
    std::list<std::shared_ptr<StatementAstNode>>& statementsToExplore,
    StatementAstNode* statementPointer) noexcept {

  const auto blockStatementPointer = dynamic_cast<BlockStatementAstNode*>(statementPointer);
  const auto declareStatementPointer = dynamic_cast<DeclareStatementAstNode*>(statementPointer);
  const auto assignStatementPointer = dynamic_cast<AssignStatementAstNode*>(statementPointer);
  const auto ifStatementPointer = dynamic_cast<IfStatementAstNode*>(statementPointer);

  if (blockStatementPointer != nullptr) {
    for (auto const& statement : blockStatementPointer->statements) {
      statementsToExplore.push_back(statement);
    }
  } else if (declareStatementPointer != nullptr) {
    exploreExpressionNonLoops(statementsToExplore, declareStatementPointer->expression.get());

  } else if (assignStatementPointer != nullptr) {
    exploreExpressionNonLoops(statementsToExplore, assignStatementPointer->expression.get());

  } else if (ifStatementPointer != nullptr) {
    exploreStatementsNonLoops(statementsToExplore, ifStatementPointer->ifStatement.get());

    if (ifStatementPointer->elseStatement) {
      exploreStatementsNonLoops(statementsToExplore, ifStatementPointer->elseStatement.value().get());
    }
  }
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

    if (breakStatementPointer != nullptr) {
      this->reportError(breakStatementPointer->breakToken,
        "Break statements may only be used from within loops.");
      noError = false;

    } else {
      exploreStatementsNonLoops(statementsToExplore, statement.get());
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