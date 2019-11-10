#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

namespace frontend {

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

};

#endif