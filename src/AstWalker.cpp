#include "AstWalker.hpp"

// abstract nodes
void AstWalker::visitStatementAstNode(StatementAstNode* node) noexcept {

  auto declare = dynamic_cast<DeclareStatementAstNode*>(node);
  if (declare != nullptr) {
    this->visitDeclareStatementAstNode(declare);
    return;
  }

  auto assign = dynamic_cast<AssignStatementAstNode*>(node);
  if (assign != nullptr) {
    this->visitAssignStatementAstNode(assign);
    return;
  }

  auto ifStmt = dynamic_cast<IfStatementAstNode*>(node);
  if (ifStmt != nullptr) {
    this->visitIfStatementAstNode(ifStmt);
    return;
  }

  auto whileStmt = dynamic_cast<WhileStatementAstNode*>(node);
  if (whileStmt != nullptr) {
    this->visitWhileStatementAstNode(whileStmt);
    return;
  }

  auto breakStmt = dynamic_cast<BreakStatementAstNode*>(node);
  if (breakStmt != nullptr) {
    this->visitBreakStatementAstNode(breakStmt);
    return;
  }

  auto returnStmt = dynamic_cast<ReturnStatementAstNode*>(node);
  if (returnStmt != nullptr) {
    this->visitReturnStatementAstNode(returnStmt);
    return;
  }

  auto block = dynamic_cast<BlockStatementAstNode*>(node);
  if (block != nullptr) {
    this->visitBlockStatementAstNode(block);
    return;
  }

  auto expr = dynamic_cast<ExpressionStatementAstNode*>(node);
  if (expr != nullptr) {
    this->visitExpressionStatementAstNode(expr);
    return;
  }

  Error::assertWithPanic(false, "visitStatementAstNode called with unexpected node type");
}

void AstWalker::visitExpressionAstNode(ExpressionAstNode* node) noexcept {

  auto literal = dynamic_cast<LiteralExpressionAstNode*>(node);
  if (literal != nullptr) {
    this->visitLiteralExpressionAstNode(literal);
    return;
  }

  auto identifier = dynamic_cast<IdentifierExpressionAstNode*>(node);
  if (identifier != nullptr) {
    this->visitIdentifierExpressionAstNode(identifier);
    return;
  }

  auto fnCall = dynamic_cast<FunctionInvocationExpressionAstNode*>(node);
  if (fnCall != nullptr) {
    this->visitFunctionInvocationExpressionAstNode(fnCall);
    return;
  }

  auto fnDefine = dynamic_cast<FunctionDeclarationExpressionAstNode*>(node);
  if (fnDefine != nullptr) {
    this->visitFunctionDeclarationExpressionAstNode(fnDefine);
    return;
  }

  auto objDef = dynamic_cast<ObjectDeclarationExpressionAstNode*>(node);
  if (objDef != nullptr) {
    this->visitObjectDeclarationExpressionAstNode(objDef);
    return;
  }

  auto builtInFn = dynamic_cast<BuiltInFunctionInvocationExpressionAstNode*>(node);
  if (builtInFn != nullptr) {
    this->visitBuiltInFunctionInvocationExpressionAstNode(builtInFn);
    return;
  }

  Error::assertWithPanic(false, "visitExpressionAstNode called with unexpected node type");
}

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

void AstWalker::visitBuiltInFunctionInvocationExpressionAstNode(BuiltInFunctionInvocationExpressionAstNode* node) noexcept {
  this->onEnterBuiltInFunctionInvocationExpressionAstNode(node);
  for (auto& expression : node->expressions) {
    this->visitExpressionAstNode(expression.get());
  }
  this->onExitBuiltInFunctionInvocationExpressionAstNode(node);
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

void AstWalker::visitExpressionStatementAstNode(ExpressionStatementAstNode* node) noexcept {
  this->onEnterExpressionStatementAstNode(node);
  this->visitExpressionAstNode(node->expression.get());
  this->onExitExpressionStatementAstNode(node);
}