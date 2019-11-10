#include "Token.hpp"

namespace frontend {

frontend::Token::Token(
    const std::string_view value,
    frontend::TokenType tokenType,
    std::size_t sourceIndex,
    std::size_t sourceLine,
    std::size_t sourceColumn) noexcept
  : value{value}
  , tokenType{tokenType}
  , sourceIndex{sourceIndex}
  , sourceLine{sourceLine}
  , sourceColumn{sourceColumn}
{}

GETTER(std::string_view, frontend::Token::Value, value);

GETTER(TokenType, frontend::Token::TokenType, tokenType);

GETTER(std::size_t, frontend::Token::SourceIndex, sourceIndex);

GETTER(std::size_t, frontend::Token::SourceLine, sourceLine);

GETTER(std::size_t, frontend::Token::SourceColumn, sourceColumn);

};