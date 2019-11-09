#ifndef AST_WALKER_HPP
#define AST_WALKER_HPP

#include "Ast.hpp"
#include "Error.hpp"

class AstWalker {
public:
  explicit AstWalker() noexcept = default;

  virtual ~AstWalker() noexcept = default;

  // abstract nodes
  virtual void visitStatementAstNode(StatementAstNode* node) noexcept;
  virtual void visitExpressionAstNode(ExpressionAstNode* node) noexcept;

  // concrete walkers
  virtual void visitScriptAstNode(ScriptAstNode* node) noexcept;
  virtual void visitDeclareStatementAstNode(DeclareStatementAstNode* node) noexcept;
  virtual void visitAssignStatementAstNode(AssignStatementAstNode* node) noexcept;
  virtual void visitIfStatementAstNode(IfStatementAstNode* node) noexcept;
  virtual void visitWhileStatementAstNode(WhileStatementAstNode* node) noexcept;
  virtual void visitBreakStatementAstNode(BreakStatementAstNode* node) noexcept;
  virtual void visitReturnStatementAstNode(ReturnStatementAstNode* node) noexcept;
  virtual void visitBlockStatementAstNode(BlockStatementAstNode* node) noexcept;
  virtual void visitLiteralExpressionAstNode(LiteralExpressionAstNode* node) noexcept;
  virtual void visitIdentifierExpressionAstNode(IdentifierExpressionAstNode* node) noexcept;
  virtual void visitFunctionInvocationExpressionAstNode(FunctionInvocationExpressionAstNode* node) noexcept;
  virtual void visitBuiltInFunctionInvocationExpressionAstNode(BuiltInFunctionInvocationExpressionAstNode* node) noexcept;
  virtual void visitFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode* node) noexcept;
  virtual void visitObjectDeclarationExpressionAstNode(ObjectDeclarationExpressionAstNode* node) noexcept;
  virtual void visitExpressionStatementAstNode(ExpressionStatementAstNode* node) noexcept;

  // enter callbacks
  virtual void onEnterScriptAstNode(ScriptAstNode*  /*node*/) noexcept {}
  virtual void onEnterDeclareStatementAstNode(DeclareStatementAstNode*  /*node*/) noexcept {}
  virtual void onEnterAssignStatementAstNode(AssignStatementAstNode* /*node*/) noexcept {}
  virtual void onEnterIfStatementAstNode(IfStatementAstNode*  /*node*/) noexcept {}
  virtual void onEnterWhileStatementAstNode(WhileStatementAstNode*  /*node*/) noexcept {}
  virtual void onEnterBreakStatementAstNode(BreakStatementAstNode*  /*node*/) noexcept {}
  virtual void onEnterReturnStatementAstNode(ReturnStatementAstNode*  /*node*/) noexcept {}
  virtual void onEnterBlockStatementAstNode(BlockStatementAstNode*  /*node*/) noexcept {}
  virtual void onEnterLiteralExpressionAstNode(LiteralExpressionAstNode*  /*node*/) noexcept {}
  virtual void onEnterIdentifierExpressionAstNode(IdentifierExpressionAstNode*  /*node*/) noexcept {}
  virtual void onEnterFunctionInvocationExpressionAstNode(FunctionInvocationExpressionAstNode*  /*node*/) noexcept {}
  virtual void onEnterBuiltInFunctionInvocationExpressionAstNode(BuiltInFunctionInvocationExpressionAstNode*  /*node*/) noexcept {}
  virtual void onEnterFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode*  /*node*/) noexcept {}
  virtual void onEnterObjectDeclarationExpressionAstNode(ObjectDeclarationExpressionAstNode*  /*node*/) noexcept {}
  virtual void onEnterExpressionStatementAstNode(ExpressionStatementAstNode*) noexcept {}

  // exit callbacks
  virtual void onExitScriptAstNode(ScriptAstNode*  /*node*/) noexcept {}
  virtual void onExitDeclareStatementAstNode(DeclareStatementAstNode*  /*node*/) noexcept {}
  virtual void onExitAssignStatementAstNode(AssignStatementAstNode* /*node*/) noexcept {}
  virtual void onExitIfStatementAstNode(IfStatementAstNode*  /*node*/) noexcept {}
  virtual void onExitWhileStatementAstNode(WhileStatementAstNode*  /*node*/) noexcept {}
  virtual void onExitBreakStatementAstNode(BreakStatementAstNode*  /*node*/) noexcept {}
  virtual void onExitReturnStatementAstNode(ReturnStatementAstNode*  /*node*/) noexcept {}
  virtual void onExitBlockStatementAstNode(BlockStatementAstNode*  /*node*/) noexcept {}
  virtual void onExitLiteralExpressionAstNode(LiteralExpressionAstNode*  /*node*/) noexcept {}
  virtual void onExitIdentifierExpressionAstNode(IdentifierExpressionAstNode*  /*node*/) noexcept {}
  virtual void onExitFunctionInvocationExpressionAstNode(FunctionInvocationExpressionAstNode*  /*node*/) noexcept {}
  virtual void onExitBuiltInFunctionInvocationExpressionAstNode(BuiltInFunctionInvocationExpressionAstNode*  /*node*/) noexcept {}
  virtual void onExitFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode*  /*node*/) noexcept {}
  virtual void onExitObjectDeclarationExpressionAstNode(ObjectDeclarationExpressionAstNode*  /*node*/) noexcept {}
  virtual void onExitExpressionStatementAstNode(ExpressionStatementAstNode*) noexcept {}
};

#endif