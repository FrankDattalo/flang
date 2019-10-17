#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "lib.hpp"

#include "Stringable.hpp"

enum class TokenType {
  Unknown,
  EndOfFile,
  LeftCurly,
  RightCurly,
  NewLine,
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
};

class Token : public Stringable {
public:
  const std::string value;
  const TokenType   tokenType;
  const std::size_t sourceIndex;
  const std::size_t sourceLine;
  const std::size_t sourceColumn;
public:
  explicit Token(
    std::string  value,
    TokenType   tokenType,
    std::size_t sourceIndex,
    std::size_t sourceLine,
    std::size_t sourceColumn) noexcept
  : value{std::move(value)}, tokenType{tokenType},
    sourceIndex{sourceIndex}, sourceLine{sourceLine},
    sourceColumn{sourceColumn}
  {}

  virtual ~Token() = default;

  const std::string toString() const noexcept override;
};

#endif