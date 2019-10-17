#include "Token.hpp"

constexpr std::string_view TokenTypeToString[] = {
  "Unknown",
  "EndOfFile",
  "LeftCurly",
  "RightCurly",
  "NewLine",
};

const std::string Token::toString() const noexcept {
  return (
    "Token(" 
      "reader: " + this->value +
      ", tokenType: " + std::string{TokenTypeToString[static_cast<int>(this->tokenType)]} +
      ", sourceIndex: " + std::to_string(this->sourceIndex) +
      ", sourceLine: " + std::to_string(this->sourceLine) +
      ", sourceColumn " + std::to_string(this->sourceColumn) +
  ")"
  );
}