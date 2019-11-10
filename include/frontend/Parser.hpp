#ifndef PARSER_HPP
#define PARSER_HPP

#include "utils/lib.hpp"
#include "utils/error.hpp"
#include "frontend/Ast.hpp"
#include "frontend/TokenBuffer.hpp"

namespace frontend {

class Parser {
private:
  TokenBuffer tokenBuffer;
  std::ostream & out;

public:
  explicit Parser(std::string_view, std::ostream&) noexcept;

  const std::unique_ptr<ScriptAstNode>
  ParseScript() noexcept;

  const std::unique_ptr<StatementAstNode>
  ParseStatement() noexcept;

  const std::unique_ptr<IfStatementAstNode>
  ParseIfStatement() noexcept;

  const std::unique_ptr<DeclareStatementAstNode>
  ParseDeclareStatement() noexcept;

  const std::unique_ptr<WhileStatementAstNode>
  ParseWhileStatement() noexcept;

  const std::unique_ptr<BreakStatementAstNode>
  ParseBreakStatement() noexcept;

  const std::unique_ptr<ReturnStatementAstNode>
  ParseReturnStatement() noexcept;

  const std::unique_ptr<AssignStatementAstNode>
  ParseAssignStatement() noexcept;

  const std::unique_ptr<BlockStatementAstNode>
  ParseBlockStatement() noexcept;

  const std::unique_ptr<ExpressionStatementAstNode>
  ParseExpressionStatement() noexcept;

  const std::unique_ptr<ExpressionAstNode>
  ParseExpression() noexcept;

  const std::unique_ptr<LiteralExpressionAstNode>
  ParseLiteralExpression() noexcept;

  const std::unique_ptr<IdentifierExpressionAstNode>
  ParseIdentiferExpression() noexcept;

  const std::unique_ptr<FunctionDeclarationExpressionAstNode>
  ParseFunctionDeclarationExpression() noexcept;

  const std::unique_ptr<ObjectDeclarationExpressionAstNode>
  ParseObjectDeclarationExpression() noexcept;

  const std::unique_ptr<ExpressionAstNode>
  ParseIdentifierOrFunctionInvocationExpression() noexcept;

  const std::unique_ptr<StatementAstNode>
  ParseAssignOrExpressionStatement() noexcept;

  const std::unique_ptr<FunctionInvocationExpressionAstNode>
  ParseFunctionInvocationExpression() noexcept;

  const std::unique_ptr<BuiltInFunctionInvocationExpressionAstNode>
  ParseBuiltInFunctionInvocationExpression() noexcept;

private:
  void ReportError(
    const Token& token,
    const std::string & errorMessage) noexcept;

  void SkipWhiteSpace() noexcept;

  const std::unique_ptr<Token> Expect(
    TokenType type,
    bool & error,
    const std::string& errorMessage) noexcept;
};

};

#endif
