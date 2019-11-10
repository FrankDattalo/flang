#include "frontend/AstWalker.hpp"

void frontend::AstWalker::OnEnterScriptAstNode(const ScriptAstNode&) const noexcept {}
void frontend::AstWalker::OnExitScriptAstNode(const ScriptAstNode&) const noexcept {}
void frontend::AstWalker::VisitScriptAstNode(const ScriptAstNode& node) const noexcept {
    this->OnEnterScriptAstNode(node);
    for (const auto& statement : node.Statements()) {
      this->VisitStatementAstNode(*statement);
    }
    this->OnExitScriptAstNode(node);
}

void frontend::AstWalker::VisitStatementAstNode(const StatementAstNode& node) const noexcept {
  node.Accept(*this);
}

void frontend::AstWalker::VisitExpressionAstNode(const ExpressionAstNode& node) const noexcept {
  node.Accept(*this);
}

void frontend::AstWalker::OnEnterDeclareStatementAstNode(const DeclareStatementAstNode&) const noexcept {}
void frontend::AstWalker::OnExitDeclareStatementAstNode(const DeclareStatementAstNode&) const noexcept {}
void frontend::AstWalker::VisitDeclareStatementAstNode(const DeclareStatementAstNode& node) const noexcept {
  this->OnEnterDeclareStatementAstNode(node);
  this->VisitExpressionAstNode(*node.Expression());
  this->OnExitDeclareStatementAstNode(node);
}

void frontend::AstWalker::OnEnterAssignStatementAstNode(const AssignStatementAstNode&) const noexcept {}
void frontend::AstWalker::OnExitAssignStatementAstNode(const AssignStatementAstNode&) const noexcept {}
void frontend::AstWalker::VisitAssignStatementAstNode(const AssignStatementAstNode& node) const noexcept {
  this->OnEnterAssignStatementAstNode(node);
  this->VisitExpressionAstNode(*node.Expression());
  this->OnExitAssignStatementAstNode(node);
}

void frontend::AstWalker::OnEnterIfStatementAstNode(const IfStatementAstNode&) const noexcept {}
void frontend::AstWalker::OnAfterExpressionIfStatementAstNode(const IfStatementAstNode&) const noexcept {}
void frontend::AstWalker::OnBeforeElseIfStatementAstNode(const IfStatementAstNode&) const noexcept {}
void frontend::AstWalker::OnExitIfStatementAstNode(const IfStatementAstNode&) const noexcept {}
void frontend::AstWalker::VisitIfStatementAstNode(const IfStatementAstNode& node) const noexcept {
  this->OnEnterIfStatementAstNode(node);
  this->VisitExpressionAstNode(*node.Expression());
  this->OnAfterExpressionIfStatementAstNode(node);
  this->VisitStatementAstNode(*node.IfStatement());
  if (node.ElseStatement() != nullptr) {
    this->OnBeforeElseIfStatementAstNode(node);
    this->VisitStatementAstNode(*node.ElseStatement());
  }
  this->OnExitIfStatementAstNode(node);
}

void frontend::AstWalker::OnEnterWhileStatementAstNode(const WhileStatementAstNode&) const noexcept {}
void frontend::AstWalker::OnExitWhileStatementAstNode(const WhileStatementAstNode&) const noexcept {}
void frontend::AstWalker::OnAfterExpressionWhileStatementAstNode(const WhileStatementAstNode&) const noexcept {}
void frontend::AstWalker::VisitWhileStatementAstNode(const WhileStatementAstNode& node) const noexcept {
  this->OnEnterWhileStatementAstNode(node);
  this->VisitExpressionAstNode(*node.Expression());
  this->OnAfterExpressionWhileStatementAstNode(node);
  this->VisitStatementAstNode(*node.Statement());
  this->OnExitWhileStatementAstNode(node);
}

void frontend::AstWalker::OnBreakStatementAstNode(const BreakStatementAstNode&) const noexcept {}
void frontend::AstWalker::VisitBreakStatementAstNode(const BreakStatementAstNode& node) const noexcept {
  this->OnBreakStatementAstNode(node);
}

void frontend::AstWalker::OnEnterReturnStatementAstNode(const ReturnStatementAstNode&) const noexcept {}
void frontend::AstWalker::OnExitReturnStatementAstNode(const ReturnStatementAstNode&) const noexcept {}
void frontend::AstWalker::VisitReturnStatementAstNode(const ReturnStatementAstNode& node) const noexcept {
  this->OnEnterReturnStatementAstNode(node);
  if (node.Expression() != nullptr) {
    this->VisitExpressionAstNode(*node.Expression());
  }
  this->OnExitReturnStatementAstNode(node);
}

void frontend::AstWalker::OnEnterBlockStatementAstNode(const BlockStatementAstNode&) const noexcept {}
void frontend::AstWalker::OnExitBlockStatementAstNode(const BlockStatementAstNode&) const noexcept {}
void frontend::AstWalker::VisitBlockStatementAstNode(const BlockStatementAstNode& node) const noexcept {
  this->OnEnterBlockStatementAstNode(node);
  for (const auto& stmt : node.Statements()) {
    this->VisitStatementAstNode(*stmt);
  }
  this->OnExitBlockStatementAstNode(node);
}

void frontend::AstWalker::OnLiteralExpressionAstNode(const LiteralExpressionAstNode&) const noexcept {}
void frontend::AstWalker::VisitLiteralExpressionAstNode(const LiteralExpressionAstNode& node) const noexcept {
  this->OnLiteralExpressionAstNode(node);
}

void frontend::AstWalker::OnIdentifierExpressionAstNode(const IdentifierExpressionAstNode&) const noexcept {}
void frontend::AstWalker::VisitIdentifierExpressionAstNode(const IdentifierExpressionAstNode& node) const noexcept {
  this->OnIdentifierExpressionAstNode(node);
}

void frontend::AstWalker::OnEnterFunctionInvocationExpressionAstNode(const FunctionInvocationExpressionAstNode&) const noexcept {}
void frontend::AstWalker::OnExitFunctionInvocationExpressionAstNode(const FunctionInvocationExpressionAstNode&) const noexcept {}
void frontend::AstWalker::VisitFunctionInvocationExpressionAstNode(const FunctionInvocationExpressionAstNode& node) const noexcept {
  this->OnEnterFunctionInvocationExpressionAstNode(node);
  for (const auto& expression : node.Expressions()) {
    this->VisitExpressionAstNode(*expression);
  }
  this->OnExitFunctionInvocationExpressionAstNode(node);
}

void frontend::AstWalker::OnEnterBuiltInFunctionInvocationExpressionAstNode(const BuiltInFunctionInvocationExpressionAstNode&) const noexcept {}
void frontend::AstWalker::OnExitBuiltInFunctionInvocationExpressionAstNode(const BuiltInFunctionInvocationExpressionAstNode&) const noexcept {}
void frontend::AstWalker::VisitBuiltInFunctionInvocationExpressionAstNode(const BuiltInFunctionInvocationExpressionAstNode& node) const noexcept {
  this->OnEnterBuiltInFunctionInvocationExpressionAstNode(node);
  for (const auto& expression : node.Expressions()) {
    this->VisitExpressionAstNode(*expression);
  }
  this->OnExitBuiltInFunctionInvocationExpressionAstNode(node);
}

void frontend::AstWalker::OnEnterFunctionDeclarationExpressionAstNode(const FunctionDeclarationExpressionAstNode&) const noexcept {}
void frontend::AstWalker::OnExitFunctionDeclarationExpressionAstNode(const FunctionDeclarationExpressionAstNode&) const noexcept {}
void frontend::AstWalker::VisitFunctionDeclarationExpressionAstNode(const FunctionDeclarationExpressionAstNode& node) const noexcept {
  this->OnEnterFunctionDeclarationExpressionAstNode(node);
  for (const auto & statement : node.Statements()) {
    this->VisitStatementAstNode(*statement);
  }
  this->OnExitFunctionDeclarationExpressionAstNode(node);
}

void frontend::AstWalker::OnEnterObjectDeclarationExpressionAstNode(const ObjectDeclarationExpressionAstNode&) const noexcept {}
void frontend::AstWalker::OnExitObjectDeclarationExpressionAstNode(const ObjectDeclarationExpressionAstNode&) const noexcept {}
void frontend::AstWalker::VisitObjectDeclarationExpressionAstNode(const ObjectDeclarationExpressionAstNode& node) const noexcept {
  this->OnEnterObjectDeclarationExpressionAstNode(node);
  for (const auto & kv : node.KeyValues()) {
    this->VisitExpressionAstNode(*kv.second);
  }
  this->OnExitObjectDeclarationExpressionAstNode(node);
}

void frontend::AstWalker::OnEnterExpressionStatementAstNode(const ExpressionStatementAstNode&) const noexcept {}
void frontend::AstWalker::OnExitExpressionStatementAstNode(const ExpressionStatementAstNode&) const noexcept {}
void frontend::AstWalker::VisitExpressionStatementAstNode(const ExpressionStatementAstNode& node) const noexcept {
  this->OnEnterExpressionStatementAstNode(node);
  this->VisitExpressionAstNode(*node.Expression());
  this->OnExitExpressionStatementAstNode(node);
}
