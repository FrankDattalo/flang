#ifndef AST_HPP
#define AST_HPP

#include "utils/lib.hpp"
#include "utils/macros.hpp"
#include "frontend/Token.hpp"

namespace frontend {

class AstWalker;

class StatementAstNode {
public:
  virtual ~StatementAstNode() = default;

  virtual void Accept(const AstWalker& walker) const = 0;
};

class ExpressionAstNode {
public:
  virtual ~ExpressionAstNode() = default;

  virtual void Accept(const AstWalker& walker) const = 0;
};

class ScriptAstNode {
private:
  const std::vector<std::unique_ptr<StatementAstNode>> statements;

  NOT_COPYABLE(ScriptAstNode);

public:
  explicit ScriptAstNode(std::vector<std::unique_ptr<StatementAstNode>>) noexcept;

  HAS_GETTER(std::vector<std::unique_ptr<StatementAstNode>>, Statements);
};

class DeclareStatementAstNode : public StatementAstNode {
private:
  const std::unique_ptr<Token> identifier;
  const std::unique_ptr<ExpressionAstNode> expression;

  NOT_COPYABLE(DeclareStatementAstNode);

public:
  explicit DeclareStatementAstNode(std::unique_ptr<Token>, std::unique_ptr<ExpressionAstNode>) noexcept;

  HAS_GETTER(std::unique_ptr<Token>, Identifier);

  HAS_GETTER(std::unique_ptr<ExpressionAstNode>, Expression);
};

class IfStatementAstNode : public StatementAstNode {
private:
  const std::unique_ptr<ExpressionAstNode> expression;
  const std::unique_ptr<StatementAstNode> ifStatement;
  const std::unique_ptr<StatementAstNode> elseStatement;

  NOT_COPYABLE(IfStatementAstNode);

public:
  explicit IfStatementAstNode(std::unique_ptr<ExpressionAstNode>, std::unique_ptr<StatementAstNode>, std::unique_ptr<StatementAstNode>) noexcept;

  explicit IfStatementAstNode(std::unique_ptr<ExpressionAstNode>, std::unique_ptr<StatementAstNode>) noexcept;

  HAS_GETTER(std::unique_ptr<ExpressionAstNode>, Expression);

  HAS_GETTER(std::unique_ptr<StatementAstNode>, IfStatement);

  HAS_GETTER(std::unique_ptr<StatementAstNode>, ElseStatement);
};

class WhileStatementAstNode : public StatementAstNode {
private:
  const std::unique_ptr<ExpressionAstNode> expression;
  const std::unique_ptr<StatementAstNode> statement;

  NOT_COPYABLE(WhileStatementAstNode);

public:
  explicit WhileStatementAstNode(std::unique_ptr<ExpressionAstNode>, std::unique_ptr<StatementAstNode>) noexcept;

  HAS_GETTER(std::unique_ptr<ExpressionAstNode>, Expression);

  HAS_GETTER(std::unique_ptr<StatementAstNode>, Statement);
};

class BreakStatementAstNode : public StatementAstNode {
private:
  const std::unique_ptr<Token> token;

  NOT_COPYABLE(BreakStatementAstNode);

public:
  explicit BreakStatementAstNode(std::unique_ptr<Token>) noexcept;

  HAS_GETTER(std::unique_ptr<Token>, Token);
};

class ReturnStatementAstNode : public StatementAstNode {
private:
  const std::unique_ptr<Token> token;
  const std::unique_ptr<ExpressionAstNode> expression;

  NOT_COPYABLE(ReturnStatementAstNode);

public:
  explicit ReturnStatementAstNode(std::unique_ptr<Token>, std::unique_ptr<ExpressionAstNode>) noexcept;

  explicit ReturnStatementAstNode(std::unique_ptr<Token>) noexcept;

  HAS_GETTER(std::unique_ptr<Token>, Token);

  HAS_GETTER(std::unique_ptr<ExpressionAstNode>, Expression);
};

class AssignStatementAstNode : public StatementAstNode {
private:
  const std::unique_ptr<Token> identifier;
  const std::unique_ptr<ExpressionAstNode> expression;

  NOT_COPYABLE(AssignStatementAstNode);

public:
  explicit AssignStatementAstNode(std::unique_ptr<Token>, std::unique_ptr<ExpressionAstNode>) noexcept;

  HAS_GETTER(std::unique_ptr<Token>, Identifier);

  HAS_GETTER(std::unique_ptr<ExpressionAstNode>, Expression);
};

class BlockStatementAstNode : public StatementAstNode {
private:
  const std::vector<std::unique_ptr<StatementAstNode>> statements;

  NOT_COPYABLE(BlockStatementAstNode);

public:
  explicit BlockStatementAstNode(std::vector<std::unique_ptr<StatementAstNode>>) noexcept;

  HAS_GETTER(std::vector<std::unique_ptr<StatementAstNode>>, Statements);
};

class ExpressionStatementAstNode : public StatementAstNode {
private:
  const std::unique_ptr<ExpressionAstNode> expression;

  NOT_COPYABLE(ExpressionStatementAstNode);

public:
  explicit ExpressionStatementAstNode(std::unique_ptr<ExpressionAstNode>) noexcept;

  HAS_GETTER(std::unique_ptr<ExpressionAstNode>, Expression);
};

class LiteralExpressionAstNode : public ExpressionAstNode {
private:
  const std::unique_ptr<Token> token;

  NOT_COPYABLE(LiteralExpressionAstNode);

public:
  explicit LiteralExpressionAstNode(std::unique_ptr<Token>) noexcept;

  HAS_GETTER(std::unique_ptr<Token>, Token);
};

class IdentifierExpressionAstNode : public ExpressionAstNode {
private:
  const std::unique_ptr<Token> token;

  NOT_COPYABLE(IdentifierExpressionAstNode);

public:
  explicit IdentifierExpressionAstNode(std::unique_ptr<Token>) noexcept;

  HAS_GETTER(std::unique_ptr<Token>, Token);
};

class FunctionInvocationExpressionAstNode : public ExpressionAstNode {
private:
  const std::unique_ptr<Token> identifier;
  const std::vector<std::unique_ptr<ExpressionAstNode>> expressions;

  NOT_COPYABLE(FunctionInvocationExpressionAstNode);

public:
  explicit FunctionInvocationExpressionAstNode(std::unique_ptr<Token>, std::vector<std::unique_ptr<ExpressionAstNode>>);

  HAS_GETTER(std::unique_ptr<Token>, Identifier);

  HAS_GETTER(std::vector<std::unique_ptr<ExpressionAstNode>>, Expressions);
};

class BuiltInFunctionInvocationExpressionAstNode : public ExpressionAstNode {
private:
  const std::unique_ptr<Token> identifier;
  const std::vector<std::unique_ptr<ExpressionAstNode>> expressions;

  NOT_COPYABLE(BuiltInFunctionInvocationExpressionAstNode);

public:
  explicit BuiltInFunctionInvocationExpressionAstNode(std::unique_ptr<Token>, std::vector<std::unique_ptr<ExpressionAstNode>>) noexcept;

  HAS_GETTER(std::unique_ptr<Token>, Identifier);

  HAS_GETTER(std::vector<std::unique_ptr<ExpressionAstNode>>, Expressions);
};

class FunctionDeclarationExpressionAstNode : public ExpressionAstNode {
private:
  const std::vector<std::unique_ptr<Token>> parameters;
  const std::vector<std::unique_ptr<StatementAstNode>> statements;

  NOT_COPYABLE(FunctionDeclarationExpressionAstNode);

public:
  explicit FunctionDeclarationExpressionAstNode(std::vector<std::unique_ptr<Token>>, std::vector<std::unique_ptr<StatementAstNode>>) noexcept;

  HAS_GETTER(std::vector<std::unique_ptr<Token>>, Parameters);

  HAS_GETTER(std::vector<std::unique_ptr<StatementAstNode>>, Statements);
};

class ObjectDeclarationExpressionAstNode : public ExpressionAstNode {
private:
  const std::vector<std::pair<std::unique_ptr<Token>, std::unique_ptr<ExpressionAstNode>>> keyValues;

  NOT_COPYABLE(ObjectDeclarationExpressionAstNode);

public:
  explicit ObjectDeclarationExpressionAstNode(std::vector<std::pair<std::unique_ptr<Token>, std::unique_ptr<ExpressionAstNode>>>) noexcept;

  // Sadly C/C++ macros leave much to be desired, and because of the comma in this, we cannot use our macro
  const std::vector<std::pair<std::unique_ptr<Token>,std::unique_ptr<ExpressionAstNode>>>& KeyValues() const noexcept;
};

};

#endif