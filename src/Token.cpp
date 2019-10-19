#include "Token.hpp"






const char * TokenTypeToString[] = {
  "Unknown",
  "EndOfFile",
  "LeftCurly",
  "RightCurly",
  "WhiteSpace",
  "LeftParen",
  "RightParen",
  "LeftBracket",
  "RightBracket",
  "Function",
  "Return",
  "Break",
  "For",
  "If",
  "Else",
  "While",
  "SemiColon",
  "Var",
  "Assign",
  "Comma",
  "BooleanLiteral",
  "UndefinedLiteral",
  "Period",
  "Colon",
  "Identifier",
  "StringLiteral",
  "IntegerLiteral",
  "FloatLiteral",
};

const std::string Token::toString() const noexcept {
  return (
    "Token("
      "value: '" + this->value +
      "', tokenType: " + Token::typeToString(this->tokenType) +
      ", sourceIndex: " + std::to_string(this->sourceIndex) +
      ", sourceLine: " + std::to_string(this->sourceLine) +
      ", sourceColumn " + std::to_string(this->sourceColumn) +
  ")"
  );
}

bool Token::isNewLine(const std::string & value) {
  return value == "\r\n" || value == "\r" || value == "\n";
}

bool Token::isNewLine() const {
  return this->tokenType == TokenType::WhiteSpace && Token::isNewLine(this->value);
}

const std::string Token::typeToString(TokenType type) noexcept {
  return TokenTypeToString[static_cast<int>(type)];
}