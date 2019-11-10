#ifndef AST_WALKER_HPP
#define AST_WALKER_HPP

#include "frontend/Ast.hpp"
#include "frontend/AstWalkerHandler.hpp"
#include "utils/error.hpp"

namespace frontend {

class AstWalker {
private:
  const AstWalkerHandler & handler;

public:
  explicit AstWalker(const AstWalkerHandler & handler) noexcept;

  void visitScriptAstNode(const ScriptAstNode& node) const noexcept;

  void visitStatementAstNode(const StatementAstNode& node) const noexcept;

  void visitExpressionAstNode(const ExpressionAstNode& node) const noexcept;

  void visitDeclareStatementAstNode(const DeclareStatementAstNode& node) const noexcept;

  void visitAssignStatementAstNode(const AssignStatementAstNode& node) const noexcept;

  void visitIfStatementAstNode(const IfStatementAstNode& node) const noexcept;

  void visitWhileStatementAstNode(const WhileStatementAstNode& node) const noexcept;

  void visitBreakStatementAstNode(const BreakStatementAstNode& node) const noexcept;

  void visitReturnStatementAstNode(const ReturnStatementAstNode& node) const noexcept;

  void visitBlockStatementAstNode(const BlockStatementAstNode& node) const noexcept;

  void visitLiteralExpressionAstNode(const LiteralExpressionAstNode& node) const noexcept;

  void visitIdentifierExpressionAstNode(const IdentifierExpressionAstNode& node) const noexcept;

  void visitFunctionInvocationExpressionAstNode(const FunctionInvocationExpressionAstNode& node) const noexcept;

  void visitBuiltInFunctionInvocationExpressionAstNode(const BuiltInFunctionInvocationExpressionAstNode& node) const noexcept;

  void visitFunctionDeclarationExpressionAstNode(const FunctionDeclarationExpressionAstNode& node) const noexcept;

  void visitObjectDeclarationExpressionAstNode(const ObjectDeclarationExpressionAstNode& node) const noexcept;

  void visitExpressionStatementAstNode(const ExpressionStatementAstNode& node) const noexcept;
};

};

#endif