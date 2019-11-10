#ifndef AST_WALKER_HPP
#define AST_WALKER_HPP

#include "frontend/Ast.hpp"
#include "utils/error.hpp"
#include "utils/macros.hpp"

namespace frontend {

class AstWalker {
private:
    NOT_COPYABLE(AstWalker);

public:

  virtual void OnEnterScriptAstNode(const ScriptAstNode& node) const noexcept;
  virtual void OnExitScriptAstNode(const ScriptAstNode& node) const noexcept;
  void VisitScriptAstNode(const ScriptAstNode& node) const noexcept;

  void VisitStatementAstNode(const StatementAstNode& node) const noexcept;

  void VisitExpressionAstNode(const ExpressionAstNode& node) const noexcept;

  virtual void OnEnterDeclareStatementAstNode(const DeclareStatementAstNode& node) const noexcept;
  virtual void OnExitDeclareStatementAstNode(const DeclareStatementAstNode& node) const noexcept;
  void VisitDeclareStatementAstNode(const DeclareStatementAstNode& node) const noexcept;

  virtual void OnEnterAssignStatementAstNode(const AssignStatementAstNode& node) const noexcept;
  virtual void OnExitAssignStatementAstNode(const AssignStatementAstNode& node) const noexcept;
  void VisitAssignStatementAstNode(const AssignStatementAstNode& node) const noexcept;

  virtual void OnEnterIfStatementAstNode(const IfStatementAstNode& node) const noexcept;
  virtual void OnAfterExpressionIfStatementAstNode(const IfStatementAstNode& node) const noexcept;
  virtual void OnBeforeElseIfStatementAstNode(const IfStatementAstNode& node) const noexcept;
  virtual void OnExitIfStatementAstNode(const IfStatementAstNode& node) const noexcept;
  void VisitIfStatementAstNode(const IfStatementAstNode& node) const noexcept;

  virtual void OnEnterWhileStatementAstNode(const WhileStatementAstNode& node) const noexcept;
  virtual void OnAfterExpressionWhileStatementAstNode(const WhileStatementAstNode& node) const noexcept;
  virtual void OnExitWhileStatementAstNode(const WhileStatementAstNode& node) const noexcept;
  void VisitWhileStatementAstNode(const WhileStatementAstNode& node) const noexcept;

  virtual void OnBreakStatementAstNode(const BreakStatementAstNode& node) const noexcept;
  void VisitBreakStatementAstNode(const BreakStatementAstNode& node) const noexcept;

  virtual void OnEnterReturnStatementAstNode(const ReturnStatementAstNode& node) const noexcept;
  virtual void OnExitReturnStatementAstNode(const ReturnStatementAstNode& node) const noexcept;
  void VisitReturnStatementAstNode(const ReturnStatementAstNode& node) const noexcept;

  virtual void OnEnterBlockStatementAstNode(const BlockStatementAstNode& node) const noexcept;
  virtual void OnExitBlockStatementAstNode(const BlockStatementAstNode& node) const noexcept;
  void VisitBlockStatementAstNode(const BlockStatementAstNode& node) const noexcept;

  virtual void OnLiteralExpressionAstNode(const LiteralExpressionAstNode& node) const noexcept;
  void VisitLiteralExpressionAstNode(const LiteralExpressionAstNode& node) const noexcept;

  virtual void OnIdentifierExpressionAstNode(const IdentifierExpressionAstNode& node) const noexcept;
  void VisitIdentifierExpressionAstNode(const IdentifierExpressionAstNode& node) const noexcept;

  virtual void OnEnterFunctionInvocationExpressionAstNode(const FunctionInvocationExpressionAstNode& node) const noexcept;
  virtual void OnExitFunctionInvocationExpressionAstNode(const FunctionInvocationExpressionAstNode& node) const noexcept;
  void VisitFunctionInvocationExpressionAstNode(const FunctionInvocationExpressionAstNode& node) const noexcept;

  virtual void OnEnterBuiltInFunctionInvocationExpressionAstNode(const BuiltInFunctionInvocationExpressionAstNode& node) const noexcept;
  virtual void OnExitBuiltInFunctionInvocationExpressionAstNode(const BuiltInFunctionInvocationExpressionAstNode& node) const noexcept;
  void VisitBuiltInFunctionInvocationExpressionAstNode(const BuiltInFunctionInvocationExpressionAstNode& node) const noexcept;

  virtual void OnEnterFunctionDeclarationExpressionAstNode(const FunctionDeclarationExpressionAstNode& node) const noexcept;
  virtual void OnExitFunctionDeclarationExpressionAstNode(const FunctionDeclarationExpressionAstNode& node) const noexcept;
  void VisitFunctionDeclarationExpressionAstNode(const FunctionDeclarationExpressionAstNode& node) const noexcept;

  virtual void OnEnterObjectDeclarationExpressionAstNode(const ObjectDeclarationExpressionAstNode& node) const noexcept;
  virtual void OnExitObjectDeclarationExpressionAstNode(const ObjectDeclarationExpressionAstNode& node) const noexcept;
  void VisitObjectDeclarationExpressionAstNode(const ObjectDeclarationExpressionAstNode& node) const noexcept;

  virtual void OnEnterExpressionStatementAstNode(const ExpressionStatementAstNode& node) const noexcept;
  virtual void OnExitExpressionStatementAstNode(const ExpressionStatementAstNode& node) const noexcept;
  void VisitExpressionStatementAstNode(const ExpressionStatementAstNode& node) const noexcept;
};

};

#endif