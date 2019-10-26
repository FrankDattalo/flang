#include "SemanticAnalyzer.hpp"

bool Scope::hasOuterScope() const {
  return this->outerScope.has_value();
}

void Scope::define(const std::string & variableName, std::shared_ptr<Token> token) {
  this->localScope.insert(std::make_pair(variableName, token));
}

std::optional<std::shared_ptr<Token>> Scope::findLocally(const std::string & variableName) {
  auto find = this->localScope.find(variableName);

  if (find == this->localScope.end()) {
    return std::nullopt;
  }

  return find->second;
}

std::optional<std::shared_ptr<Token>> Scope::find(const std::string & variableName) {

  Scope* scopeToSearch = this;

  while (true) {

    auto find = scopeToSearch->findLocally(variableName);

    if (find) {
      return find;
    }

    if (scopeToSearch->outerScope) {
      scopeToSearch = outerScope.value().get();

    } else {
      return std::nullopt;
    }
  }
}

std::optional<std::shared_ptr<Scope>> Scope::getOuterScope() {
  return this->outerScope;
}

bool SemanticAnalyzer::isValid(const std::shared_ptr<ScriptAstNode>& script) noexcept {

  SemanticAnalyzerRun run(this);

  run.visitScriptAstNode(script.get());

  // if (!this->allVariablesDefinedBeforeUse(script)) {
  //   result = false;
  // }

  // if (!this->allVariablesDeclaredOnlyOnceWithinScope(script)) {
  //   result = false;
  // }

  // if (!this->breaksOnlyInLoops(script)) {
  //   result = false;
  // }

  // if (!this->returnsOnlyInFunctions(script)) {
  //   result = false;
  // }

  return !run.hasErrors();
}

// bool hasScope(StatementAstNode* statement) {

//   const auto ifPtr = dynamic_cast<IfStatementAstNode*>(statement);
//   const auto whilePtr = dynamic_cast<WhileStatementAstNode*>(statement);
//   const auto declarePtr = dynamic_cast<DeclareStatementAstNode*>(statement);
//   const auto assignPtr = dynamic_cast<AssignStatementAstNode*>(statement);

//   if (ifPtr != nullptr) {
//     return true;
//   }

//   if (whilePtr != nullptr) {
//     return true;
//   }

//   ExpressionAstNode* expression = nullptr;

//   if (assignPtr != nullptr) {
//     expression = assignPtr->expression.get();
//   }

//   if (declarePtr != nullptr) {
//     expression = declarePtr->expression.get();
//   }

//   if (expression == nullptr) {
//     return false;
//   }

//   const auto fn = dynamic_cast<FunctionDeclarationExpressionAstNode*>(expression);
//   const auto obj = dynamic_cast<ObjectDeclarationExpressionAstNode*>(expression);

//   if (fn != nullptr) {
//     return true;
//   }

//   if (obj != nullptr) {
//     return true;
//   }

//   return false;
// }

// bool allVariablesDefinedBeforeUse(
//     const std::vector<std::shared_ptr<StatementAstNode>>&  /*statements*/,
//     const std::shared_ptr<Scope>&  /*scope*/) {

//   // when we encouter a block, define a new scope, link the outer scope, and descend there
//   // after descending, pop the scope and continue looping

//   // when we encouter an assign or a declare which are a function definition, link the outer scope,
//   // add the parameters as defined variables within the new scope, and descend there
//   // after descending, pop the scope and continue looping

//   // when we encounter an if statement, check the if condition, link the outer scope, and descend there
//   // after descending pop the scope, if the if statement has an else, descend there

//   // when we encounter a while statement, check the condition, link the outer scope and descend there

//   // when we encounter a return statement, if the condition is present, descend there

//   // when we encoutner an assign or declare of an object literal, for each value, link the outer scope
//   // add the parameters as defined variables in the new scope, and descend there,
//   // after descending pop the scope and conitinue looping

//   // when we encounter an assign or declare of a function call or identifier, validate that the
//   // identifier or function identifier is defined.

//   return true;
// }

// bool SemanticAnalyzer::allVariablesDefinedBeforeUse(const std::shared_ptr<ScriptAstNode>&  /*script*/) noexcept {
//   return true;
// }

// bool SemanticAnalyzer::returnsOnlyInFunctions(const std::shared_ptr<ScriptAstNode>& script) noexcept {

//   std::list<std::shared_ptr<StatementAstNode>> statementsToExplore;

//   for (auto const& statement : script->statements) {
//     statementsToExplore.push_back(statement);
//   }

//   bool noError = true;

//   while (!statementsToExplore.empty()) {
//     const auto statement = statementsToExplore.front();

//     statementsToExplore.pop_front();

//     const auto statementPointer = statement.get();

//     const auto returnStatementPointer = dynamic_cast<ReturnStatementAstNode*>(statementPointer);
//     // things which we will search
//     const auto blockStatementPointer = dynamic_cast<BlockStatementAstNode*>(statementPointer);

//     if (returnStatementPointer != nullptr) {
//       this->reportError(returnStatementPointer->returnToken,
//         "Return statements may only be used from within function bodies.");

//       noError = false;

//     } else if (blockStatementPointer != nullptr) {
//       for (auto const& statement : blockStatementPointer->statements) {
//         statementsToExplore.push_back(statement);
//       }
//     }
//   }

//   return noError;
// }

// void exploreExpressionNonLoops(
//     std::list<std::shared_ptr<StatementAstNode>>& statementsToExplore,
//     ExpressionAstNode* expressionPointer) noexcept {

//   const auto fn = dynamic_cast<FunctionDeclarationExpressionAstNode*>(expressionPointer);
//   const auto obj = dynamic_cast<ObjectDeclarationExpressionAstNode*>(expressionPointer);

//   if (fn != nullptr) {
//     for (auto const& statement : fn->statements) {
//       statementsToExplore.push_back(statement);
//     }
//   } else if (obj != nullptr) {
//     for (auto const& kv : obj->keyValues) {
//       auto const value = kv.second.get();
//       exploreExpressionNonLoops(statementsToExplore, value);
//     }
//   }
// }

// void exploreStatementsNonLoops(
//     std::list<std::shared_ptr<StatementAstNode>>& statementsToExplore,
//     StatementAstNode* statementPointer) noexcept {

//   const auto blockStatementPointer = dynamic_cast<BlockStatementAstNode*>(statementPointer);
//   const auto declareStatementPointer = dynamic_cast<DeclareStatementAstNode*>(statementPointer);
//   const auto assignStatementPointer = dynamic_cast<AssignStatementAstNode*>(statementPointer);
//   const auto ifStatementPointer = dynamic_cast<IfStatementAstNode*>(statementPointer);
//   const auto returnStatementPointer = dynamic_cast<ReturnStatementAstNode*>(statementPointer);

//   if (blockStatementPointer != nullptr) {
//     for (auto const& statement : blockStatementPointer->statements) {
//       statementsToExplore.push_back(statement);
//     }
//   } else if (declareStatementPointer != nullptr) {
//     exploreExpressionNonLoops(statementsToExplore, declareStatementPointer->expression.get());

//   } else if (assignStatementPointer != nullptr) {
//     exploreExpressionNonLoops(statementsToExplore, assignStatementPointer->expression.get());

//   } else if (ifStatementPointer != nullptr) {
//     exploreStatementsNonLoops(statementsToExplore, ifStatementPointer->ifStatement.get());

//     if (ifStatementPointer->elseStatement) {
//       exploreStatementsNonLoops(statementsToExplore, ifStatementPointer->elseStatement.value().get());
//     }

//   } else if (returnStatementPointer != nullptr) {
//     if (returnStatementPointer->expression) {
//       exploreExpressionNonLoops(statementsToExplore, returnStatementPointer->expression.value().get());
//     }
//   }
// }

// bool SemanticAnalyzer::breaksOnlyInLoops(const std::shared_ptr<ScriptAstNode>& script) noexcept {

//   std::list<std::shared_ptr<StatementAstNode>> statementsToExplore;

//   for (auto const& statement : script->statements) {
//     statementsToExplore.push_back(statement);
//   }

//   bool noError = true;

//   while (!statementsToExplore.empty()) {
//     const auto statement = statementsToExplore.front();

//     statementsToExplore.pop_front();

//     const auto statementPointer = statement.get();

//     // check if this is an error
//     const auto breakStatementPointer = dynamic_cast<BreakStatementAstNode*>(statementPointer);

//     if (breakStatementPointer != nullptr) {
//       this->reportError(breakStatementPointer->breakToken,
//         "Break statements may only be used from within loops.");
//       noError = false;

//     } else {
//       exploreStatementsNonLoops(statementsToExplore, statement.get());
//     }
//   }

//   return noError;
// }

// bool checkStatementScope(std::shared_ptr<Scope>& scope, StatementAstNode* statement);

// bool checkStatementsScope(std::shared_ptr<Scope>& scope, const std::vector<std::shared_ptr<StatementAstNode>>& statements) {

//   bool noError = true;

//   for (const auto& statement : statements) {
//     auto stmtPtr = statement.get();

//     if (!checkStatementScope(scope, stmtPtr)) {
//       noError = false;
//     }
//   }

//   return noError;
// }

// bool define(std::shared_ptr<Scope>& scope, const std::string& identifier, const std::shared_ptr<Token>& token) {
//   bool noError = true;

//   if (scope->isDefinedLocally(identifier)) {
//     noError = false;

//   } else {
//     scope->define(identifier, token);
//   }

//   return noError;
// }

// bool checkFunctionOrObjectDeclaration(std::shared_ptr<Scope>& outer, ExpressionAstNode* expression) {
//   auto fn = dynamic_cast<FunctionDeclarationExpressionAstNode*>(expression);
//   auto obj = dynamic_cast<ObjectDeclarationExpressionAstNode*>(expression);

//   bool noError = true;

//   if (fn != nullptr) {

//   } else if (obj != nullptr) {
//     for (auto& kv : obj->keyValues) {
//       if (!checkFunctionOrObjectDeclaration(outer, kv.second.get())) {
//         noError = false;
//       }
//     }
//   }

//   return noError;
// }

// bool checkDeclareScope(std::shared_ptr<Scope>& scope, DeclareStatementAstNode* statement) noexcept {

//   auto identifier = statement->identifier->value;

//   bool noError = define(scope, identifier, statement->identifier);

//   if (!checkFunctionOrObjectDeclaration(scope, statement->expression.get())) {
//     noError = false;
//   }

//   return noError;
// }

// bool checkAssignScope(
//   std::shared_ptr<Scope>&  /*scope*/, AssignStatementAstNode*  /*statement*/
// ) noexcept {
//   return true;
// }

// bool checkStatementScope(std::shared_ptr<Scope>& scope, StatementAstNode* statement) {

//   auto ifStatement = dynamic_cast<IfStatementAstNode*>(statement);
//   auto blockStatement = dynamic_cast<BlockStatementAstNode*>(statement);
//   auto whileStatement = dynamic_cast<WhileStatementAstNode*>(statement);
//   auto declareStatement = dynamic_cast<DeclareStatementAstNode*>(statement);
//   auto assignStatement = dynamic_cast<AssignStatementAstNode*>(statement);

//   if (declareStatement != nullptr) {
//     return checkDeclareScope(scope, declareStatement);

//   } if (assignStatement != nullptr) {
//     return checkAssignScope(scope, assignStatement);

//   } if (blockStatement != nullptr) {
//     auto innerScope = std::make_shared<Scope>(scope);
//     return checkStatementsScope(innerScope, blockStatement->statements);

//   } else if (whileStatement != nullptr) {
//     auto innerScope = std::make_shared<Scope>(scope);
//     return checkStatementScope(innerScope, whileStatement->statement.get());

//   } else if (ifStatement != nullptr) {
//     auto innerIfScope = std::make_shared<Scope>(scope);
//     bool error = false;

//     if (!checkStatementScope(innerIfScope, ifStatement->ifStatement.get())) {
//       error = true;
//     }

//     if (ifStatement->elseStatement) {
//       auto innerElseScope = std::make_shared<Scope>(scope);
//       if (!checkStatementScope(innerElseScope, ifStatement->elseStatement.value().get())) {
//         error = true;
//       }
//     }

//     return !error;
//   }

//   return true;
// }

// bool exploreExpressionOnlyOnceDelcared(
//     std::shared_ptr<Scope>& scope,
//     std::list<std::pair<std::shared_ptr<StatementAstNode>, bool>>& statementsToExplore,
//     ExpressionAstNode* expression) {

//   auto fn = dynamic_cast<FunctionDeclarationExpressionAstNode*>(expression);
//   auto obj = dynamic_cast<ObjectDeclarationExpressionAstNode*>(expression);

//   if (fn != nullptr) {
//     auto innerScope = std::make_shared<Scope>(scope);

//   } else if (obj != nullptr) {

//   }
// }

// bool SemanticAnalyzer::allVariablesDeclaredOnlyOnceWithinScope(const std::shared_ptr<ScriptAstNode>&  script) noexcept {

//   return true;

  // auto currentScope = std::make_shared<Scope>();

  // std::list<std::pair<std::shared_ptr<StatementAstNode>, bool>> statementsToExplore;

  // for (auto const& statement : script->statements) {
  //   statementsToExplore.push_back(std::make_pair(statement, false));
  // }

  // bool noError = true;

  // while (!statementsToExplore.empty()) {
  //   const auto statement = statementsToExplore.front();

  //   statementsToExplore.pop_front();

  //   const auto statementPointer = statement.first.get();
  //   bool shouldPopScope = statement.second;

  //   auto declareStatement = dynamic_cast<DeclareStatementAstNode*>(statementPointer);
  //   auto assignStatement = dynamic_cast<AssignStatementAstNode*>(statementPointer);
  //   auto ifStatement = dynamic_cast<IfStatementAstNode*>(statementPointer);
  //   auto whileStatement = dynamic_cast<WhileStatementAstNode*>(statementPointer);
  //   auto returnStatement = dynamic_cast<ReturnStatementAstNode*>(statementPointer);
  //   auto blockStatement = dynamic_cast<BlockStatementAstNode*>(statementPointer);

  //   if (declareStatement != nullptr) {
  //     auto identifier = declareStatement->identifier->value;
  //     auto originalDeclaration = currentScope->findLocally(identifier);

  //     if (originalDeclaration) {
  //       noError = false;

  //       this->reportError(declareStatement->identifier,
  //         "Duplicate variable declaration found within the same scope.");

  //     } else {
  //       currentScope->define(identifier, declareStatement->identifier);
  //     }

  //     // exploreExpressionOnlyOnceDeclared(currentScope, statementsToExplore, declareStatement->expression.get());

  //   } else {
  //     // exploreStatementOnlyOnceDeclared(currentScope, statementsToExplore, statementPointer);
  //   }

  //   if (shouldPopScope) {
  //     auto outerScope = currentScope->getOuterScope();

  //     if (!outerScope) {
  //       ErrorUtils::panic(
  //         "Expected outer scope, to pop current scope during semantic analysis"
  //         ", but there was no outer scope defined.");

  //       return false;
  //     }

  //     currentScope = outerScope.value();
  //   }
  // }

  // return noError;
// }

void SemanticAnalyzer::reportError(
      const std::shared_ptr<Token>& token,
      const std::string & errorMessage) noexcept {

  ErrorUtils::reportErrorAtToken(
    this->out, "semantic analysis", this->reader, token, errorMessage);
}