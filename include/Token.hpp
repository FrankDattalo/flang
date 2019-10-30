#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <utility>

#include "lib.hpp"

enum class TokenType {
  Unknown,
  EndOfFile,
  LeftCurly,
  RightCurly,
  WhiteSpace,
  LeftParen,
  RightParen,
  LeftBracket,
  RightBracket,
  Function,
  Return,
  Break,
  For,
  If,
  Else,
  While,
  SemiColon,
  Var,
  Assign,
  Comma,
  BooleanLiteral,
  UndefinedLiteral,
  Period,
  Colon,
  Identifier,
  StringLiteral,
  IntegerLiteral,
  FloatLiteral,
  BuiltInFunctionName,
};

class Token {
public:
  const std::string value;
  const TokenType   tokenType;
  const std::size_t sourceIndex;
  const std::size_t sourceLine;
  const std::size_t sourceColumn;

  explicit Token(
    std::string  value,
    TokenType   tokenType,
    std::size_t sourceIndex,
    std::size_t sourceLine,
    std::size_t sourceColumn) noexcept
  : value{std::move(value)},
    tokenType{tokenType},
    sourceIndex{sourceIndex},
    sourceLine{sourceLine},
    sourceColumn{sourceColumn}
  {}

  bool isNewLine() const;

  static bool isNewLine(const std::string & value);
};

#endif