#include "Token.hpp"

bool Token::isNewLine(const std::string & value) {
  return value == "\r\n" || value == "\r" || value == "\n";
}

bool Token::isNewLine() const {
  return this->tokenType == TokenType::WhiteSpace && Token::isNewLine(this->value);
}