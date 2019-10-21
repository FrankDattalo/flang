#include "TokenBuffer.hpp"

const std::shared_ptr<Token> TokenBuffer::tokenAt(std::size_t i) noexcept {
  while (this->tokenList.size() <= i) {
    this->tokenList.push_back(this->tokenizer->currentToken());
    this->tokenizer->nextToken();
  }

  std::size_t numWalked = 0;
  std::shared_ptr<Token> token;

  for (const auto & it : this->tokenList) {
    if (numWalked == i) {
      token = it;
      break;
    }
    numWalked++;
  }

  return token;
}

void TokenBuffer::advance() noexcept {
  if (!this->tokenList.empty()) {
    this->tokenList.pop_front();

  } else {
    this->tokenizer->nextToken();
  }
}

const std::shared_ptr<Token> TokenBuffer::currentToken() noexcept {
  return this->tokenAt(0);
}

const std::shared_ptr<Token> TokenBuffer::nextToken() noexcept {
  this->advance();
  return this->tokenAt(0);
}

const std::string TokenBuffer::toString() const noexcept {
  return (
    "TokenBuffer("
    "tokenizer: " + this->tokenizer->toString() +
    ")"
  );
}

const std::shared_ptr<Tokenizer> TokenBuffer::getTokenizer() noexcept {
  return this->tokenizer;
}