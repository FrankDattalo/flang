#include "Tokenizer.hpp"

std::shared_ptr<Token> Tokenizer::nextToken() noexcept {
  return this->token;
}

std::shared_ptr<Token> Tokenizer::currentToken() const noexcept {
  return this->token;
} 

const std::string Tokenizer::toString() const noexcept {
  return (
    "Token(" 
      "reader: " + this->reader->toString() +
      ", sourceIndex: " + std::to_string(this->sourceIndex) +
      ", sourceLine: " + std::to_string(this->sourceLine) +
      ", sourceColumn " + std::to_string(this->sourceColumn) +
  ")"
  );
}