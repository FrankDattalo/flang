#ifndef AST_WALKER_HPP
#define AST_WALKER_HPP

#include "Ast.hpp"

class AstWalker {
private:
public:
  explicit AstWalker() noexcept = default;

  virtual ~AstWalker() noexcept = default;

  // abstract nodes
  void visitStatementAstNode(StatementAstNode* node) noexcept;
  void visitExpressionAstNode(ExpressionAstNode* node) noexcept;

  // concrete walkers
  void visitScriptAstNode(ScriptAstNode* node) noexcept;
  void visitDeclareStatementAstNode(DeclareStatementAstNode* node) noexcept;
  void visitAssignStatementAstNode(AssignStatementAstNode* node) noexcept;
  void visitIfStatementAstNode(IfStatementAstNode* node) noexcept;
  void visitWhileStatementAstNode(WhileStatementAstNode* node) noexcept;
  void visitBreakStatementAstNode(BreakStatementAstNode* node) noexcept;
  void visitReturnStatementAstNode(ReturnStatementAstNode* node) noexcept;
  void visitBlockStatementAstNode(BlockStatementAstNode* node) noexcept;
  void visitLiteralExpressionAstNode(LiteralExpressionAstNode* node) noexcept;
  void visitIdentifierExpressionAstNode(IdentifierExpressionAstNode* node) noexcept;
  void visitFunctionInvocationExpressionAstNode(FunctionInvocationExpressionAstNode* node) noexcept;
  void visitFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode* node) noexcept;
  void visitObjectDeclarationExpressionAstNode(ObjectDeclarationExpressionAstNode* node) noexcept;

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
  virtual void onEnterFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode*  /*node*/) noexcept {}
  virtual void onEnterObjectDeclarationExpressionAstNode(ObjectDeclarationExpressionAstNode*  /*node*/) noexcept {}

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
  virtual void onExitFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode*  /*node*/) noexcept {}
  virtual void onExitObjectDeclarationExpressionAstNode(ObjectDeclarationExpressionAstNode*  /*node*/) noexcept {}
};

#endif