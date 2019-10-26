#include "AstWalker.hpp"

#define TRY_VISIT(type) do { \
  auto casted = dynamic_cast<##type*>(node);\
  if (casted != nullptr) {\
    this->visit##type(casted);\
  }\
} while (false)

// abstract nodes
void AstWalker::visitStatementAstNode(StatementAstNode* node) noexcept {
  TRY_VISIT(DeclareStatementAstNode);
  TRY_VISIT(AssignStatementAstNode);
  TRY_VISIT(IfStatementAstNode);
  TRY_VISIT(WhileStatementAstNode);
  TRY_VISIT(BreakStatementAstNode);
  TRY_VISIT(ReturnStatementAstNode);
  TRY_VISIT(BlockStatementAstNode);
}

void AstWalker::visitExpressionAstNode(ExpressionAstNode* node) noexcept {
  TRY_VISIT(LiteralExpressionAstNode);
  TRY_VISIT(IdentifierExpressionAstNode);
  TRY_VISIT(FunctionInvocationExpressionAstNode);
  TRY_VISIT(FunctionDeclarationExpressionAstNode);
  TRY_VISIT(ObjectDeclarationExpressionAstNode);
}

#undef TRY_VISIT

// concrete walkers
void AstWalker::visitScriptAstNode(ScriptAstNode* node) noexcept {
  this->onEnterScriptAstNode(node);
  for (auto& statement : node->statements) {
    this->visitStatementAstNode(statement.get());
  }
  this->onExitScriptAstNode(node);
}

void AstWalker::visitDeclareStatementAstNode(DeclareStatementAstNode* node) noexcept {
  this->onEnterDeclareStatementAstNode(node);
  this->visitExpressionAstNode(node->expression.get());
  this->onExitDeclareStatementAstNode(node);
}

void AstWalker::visitAssignStatementAstNode(AssignStatementAstNode* node) noexcept {
  this->onEnterAssignStatementAstNode(node);
  this->visitExpressionAstNode(node->expression.get());
  this->onExitAssignStatementAstNode(node);
}

void AstWalker::visitIfStatementAstNode(IfStatementAstNode* node) noexcept {
  this->onEnterIfStatementAstNode(node);
  this->visitExpressionAstNode(node->expression.get());
  this->visitStatementAstNode(node->ifStatement.get());
  if (node->elseStatement) {
    this->visitStatementAstNode(node->elseStatement->get());
  }
  this->onExitIfStatementAstNode(node);
}

void AstWalker::visitWhileStatementAstNode(WhileStatementAstNode* node) noexcept {
  this->onEnterWhileStatementAstNode(node);
  this->visitExpressionAstNode(node->expression.get());
  this->visitStatementAstNode(node->statement.get());
  this->onExitWhileStatementAstNode(node);
}

void AstWalker::visitBreakStatementAstNode(BreakStatementAstNode* node) noexcept {
  this->onEnterBreakStatementAstNode(node);
  this->onExitBreakStatementAstNode(node);
}

void AstWalker::visitReturnStatementAstNode(ReturnStatementAstNode* node) noexcept {
  this->onEnterReturnStatementAstNode(node);
  if (node->expression) {
    this->visitExpressionAstNode(node->expression->get());
  }
  this->onExitReturnStatementAstNode(node);
}

void AstWalker::visitBlockStatementAstNode(BlockStatementAstNode* node) noexcept {
  this->onEnterBlockStatementAstNode(node);
  for (auto& stmt : node->statements) {
    this->visitStatementAstNode(stmt.get());
  }
  this->onExitBlockStatementAstNode(node);
}

void AstWalker::visitLiteralExpressionAstNode(LiteralExpressionAstNode* node) noexcept {
  this->onEnterLiteralExpressionAstNode(node);
  this->onExitLiteralExpressionAstNode(node);
}

void AstWalker::visitIdentifierExpressionAstNode(IdentifierExpressionAstNode* node) noexcept {
  this->onEnterIdentifierExpressionAstNode(node);
  this->onExitIdentifierExpressionAstNode(node);
}

void AstWalker::visitFunctionInvocationExpressionAstNode(FunctionInvocationExpressionAstNode* node) noexcept {
  this->onEnterFunctionInvocationExpressionAstNode(node);
  for (auto& expression : node->expressions) {
    this->visitExpressionAstNode(expression.get());
  }
  this->onExitFunctionInvocationExpressionAstNode(node);
}

void AstWalker::visitFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode* node) noexcept {
  this->onEnterFunctionDeclarationExpressionAstNode(node);
  for (auto & statement : node->statements) {
    this->visitStatementAstNode(statement.get());
  }
  this->onExitFunctionDeclarationExpressionAstNode(node);
}

void AstWalker::visitObjectDeclarationExpressionAstNode(ObjectDeclarationExpressionAstNode* node) noexcept {
  this->onEnterObjectDeclarationExpressionAstNode(node);
  for (auto & kv : node->keyValues) {
    this->visitExpressionAstNode(kv.second.get());
  }
  this->onExitObjectDeclarationExpressionAstNode(node);
}