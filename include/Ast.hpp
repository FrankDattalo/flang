#ifndef AST_HPP
#define AST_HPP

#include "lib.hpp"
#include "Token.hpp"

/*
script
  : statement*
  ;

statement
  : declare statement
  | if else statement
  | while statement
  | break statement
  | return statement
  | assign statment
  | block statement
  | expression statement
  ;

declare statement
  : 'var' IDENTIFIER '=' expression ';'
  ;

if else statement
  : 'if' '(' expression ')' statement
  | 'if' '(' expression ')' statement 'else' statement
  ;

while statement
  : 'while' '(' expression ')' statement
  ;

break statement
  : 'break' ';'

return statement
  : 'return' ';'
  | 'return' expression ';'
  ;

assign statement
  : IDENTIFIER '=' expression ';'
  ;

block statement
  : '{' statement* '}'
  ;

expression statement
  : expression ';'
  ;

expression
  : INTEGER LITERAL
  | BOOLEAN LITERAL
  | STRING LITERAL
  | UNDEFINED LITERAL
  | IDENTIFIER
  | function invocation
  | function declaration
  | object declaration
  | built in function invocation
  ;

function declaration
  : 'function' '(' ')' '{' statement* '}'
  | 'function' '(' IDENTIFIER ( ',' IDENTIFIER )* ')' '{' statement '}'
  ;

function invocation
  : IDENTIFIER '(' ')'
  | IDENTIFIER '(' expression (',' expression )* ')'
  ;

built in function invocation
  : BUILT_IN_FUNCTION_NAME '(' ')'
  | BUILT_IN_FUNCTION_NAME '(' expression (',' expression )* ')'
  ;

object declaration
  : '{' '}'
  | '{' IDENTIFIER ':' expression '}'
  | '{' IDENTIFIER ':' expression ( ',' IDENTIFIER ':' expression )* '}'
  ;
*/

class AstNode {
public:
  explicit AstNode() noexcept = default;
  virtual ~AstNode() noexcept = default;
};

class StatementAstNode;
class ExpressionAstNode;

class ScriptAstNode : public AstNode {
public:
  const std::vector<std::shared_ptr<StatementAstNode>> statements;

  explicit ScriptAstNode(
    std::vector<std::shared_ptr<StatementAstNode>> statements
  ) noexcept
  : statements{std::move(statements)}
  {}
};

class StatementAstNode : public AstNode {
};

class DeclareStatementAstNode : public StatementAstNode {
public:
  const std::shared_ptr<Token> identifier;
  const std::shared_ptr<ExpressionAstNode> expression;

  explicit DeclareStatementAstNode(
    std::shared_ptr<Token> identifier,
    std::shared_ptr<ExpressionAstNode> expression
  ) noexcept
  : identifier{std::move(identifier)},
    expression{std::move(expression)}
  {}
};

class IfStatementAstNode : public StatementAstNode {
public:
  const std::shared_ptr<ExpressionAstNode> expression;
  const std::shared_ptr<StatementAstNode> ifStatement;
  const std::optional<std::shared_ptr<StatementAstNode>> elseStatement;

  explicit IfStatementAstNode(
    std::shared_ptr<ExpressionAstNode>  expression,
    std::shared_ptr<StatementAstNode>  ifStatement,
    std::shared_ptr<StatementAstNode>  elseStatement
  ) noexcept
  : expression{std::move(expression)},
    ifStatement{std::move(ifStatement)},
    elseStatement{std::move(elseStatement)}
  {}

  explicit IfStatementAstNode(
    std::shared_ptr<ExpressionAstNode> expression,
    std::shared_ptr<StatementAstNode> ifStatement
  ) noexcept
  : expression{std::move(expression)},
    ifStatement{std::move(ifStatement)}
  {}
};

class WhileStatementAstNode : public StatementAstNode {
public:
  const std::shared_ptr<ExpressionAstNode> expression;
  const std::shared_ptr<StatementAstNode> statement;

  explicit WhileStatementAstNode(
    std::shared_ptr<ExpressionAstNode> expression,
    std::shared_ptr<StatementAstNode> statement
  ) noexcept
  : expression{std::move(expression)},
    statement{std::move(statement)}
  {}
};

class BreakStatementAstNode : public StatementAstNode {
public:
  const std::shared_ptr<Token> breakToken;

  explicit BreakStatementAstNode(std::shared_ptr<Token> breakToken) noexcept
  : breakToken{std::move(breakToken)}
  {}
};

class ReturnStatementAstNode : public StatementAstNode {
public:
  const std::shared_ptr<Token> returnToken;
  const std::optional<std::shared_ptr<ExpressionAstNode>> expression;

  explicit ReturnStatementAstNode(
    std::shared_ptr<Token> returnToken,
    std::shared_ptr<ExpressionAstNode> expression
  ) noexcept
  : returnToken{std::move(returnToken)}
  , expression{std::move(expression)}
  {}

  explicit ReturnStatementAstNode(std::shared_ptr<Token> returnToken) noexcept
  : returnToken{std::move(returnToken)}
  , expression{std::nullopt}
  {}
};

class AssignStatementAstNode : public StatementAstNode {
public:
  const std::shared_ptr<Token> identifier;
  const std::shared_ptr<ExpressionAstNode> expression;

  explicit AssignStatementAstNode(
    std::shared_ptr<Token> identifier,
    std::shared_ptr<ExpressionAstNode> expression
  ) noexcept
  : identifier{std::move(identifier)},
    expression{std::move(expression)}
  {}
};

class BlockStatementAstNode : public StatementAstNode {
public:
  const std::vector<std::shared_ptr<StatementAstNode>> statements;

  explicit BlockStatementAstNode(
    std::vector<std::shared_ptr<StatementAstNode>> statements
  ) noexcept
  : statements{std::move(statements)}
  {}
};

class ExpressionStatementAstNode : public StatementAstNode {
public:
  const std::shared_ptr<ExpressionAstNode> expression;

  explicit ExpressionStatementAstNode(
    std::shared_ptr<ExpressionAstNode> expression
  ) noexcept
  : expression{std::move(expression)}
  {}
};


class ExpressionAstNode : public AstNode {
};

class LiteralExpressionAstNode : public ExpressionAstNode {
public:
  const std::shared_ptr<Token> token;

  explicit LiteralExpressionAstNode(
    std::shared_ptr<Token> token
  ) noexcept
  : token{std::move(token)}
  {}
};

class IdentifierExpressionAstNode : public ExpressionAstNode {
public:
  const std::shared_ptr<Token> token;

  explicit IdentifierExpressionAstNode(
    std::shared_ptr<Token> token
  ) noexcept
  : token{std::move(token)}
  {}
};

class FunctionInvocationExpressionAstNode : public ExpressionAstNode {
public:
  const std::shared_ptr<Token> identifier;
  const std::vector<std::shared_ptr<ExpressionAstNode>> expressions;

  explicit FunctionInvocationExpressionAstNode(
    std::shared_ptr<Token> identifier,
    std::vector<std::shared_ptr<ExpressionAstNode>> expressions
  ) noexcept
  : identifier{std::move(identifier)},
    expressions{std::move(expressions)}
  {}
};

class BuiltInFunctionInvocationExpressionAstNode : public ExpressionAstNode {
public:
  const std::shared_ptr<Token> identifier;
  const std::vector<std::shared_ptr<ExpressionAstNode>> expressions;

  explicit BuiltInFunctionInvocationExpressionAstNode(
    std::shared_ptr<Token> identifier,
    std::vector<std::shared_ptr<ExpressionAstNode>> expressions
  ) noexcept
  : identifier{std::move(identifier)},
    expressions{std::move(expressions)}
  {}
};

class FunctionDeclarationExpressionAstNode : public ExpressionAstNode {
public:
  const std::vector<std::shared_ptr<Token>> parameters;
  const std::vector<std::shared_ptr<StatementAstNode>> statements;

  explicit FunctionDeclarationExpressionAstNode(
    std::vector<std::shared_ptr<Token>> parameters,
    std::vector<std::shared_ptr<StatementAstNode>> statements
  ) noexcept
  : parameters{std::move(parameters)},
    statements{std::move(statements)}
  {}
};

class ObjectDeclarationExpressionAstNode : public ExpressionAstNode {
public:
  const std::vector<std::pair<std::shared_ptr<Token>, std::shared_ptr<ExpressionAstNode>>> keyValues;

  explicit ObjectDeclarationExpressionAstNode(
    std::vector<std::pair<std::shared_ptr<Token>, std::shared_ptr<ExpressionAstNode>>> keyValues
  ) noexcept
  : keyValues{std::move(keyValues)}
  {}
};

#endif