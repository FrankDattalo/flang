#include "frontend/TokenBuffer.hpp"

namespace frontend {

frontend::TokenBuffer::TokenBuffer(std::string_view val) noexcept
  : tokenizer{val}
{}

const Token& TokenAt(std::size_t) noexcept;

std::unique_ptr<Token> frontend::TokenBuffer::Advance() noexcept {
  if (!this->tokenList.empty()) {
    auto ret = std::move(this->tokenList.front());
    this->tokenList.pop_front();
    return ret;

  } else {
    return this->tokenizer.NextToken();
  }
}

void frontend::TokenBuffer::ReportErrorAtToken(
    std::ostream & out, std::string_view phase, const Token& token, const std::string & message) {

  this->tokenizer.ReportErrorAtToken(out, phase, token, message);
}

const Token& frontend::TokenBuffer::TokenAt(std::size_t i) noexcept {
  while (this->tokenList.size() <= i) {
    this->tokenList.push_back(this->tokenizer.NextToken());
  }

  std::size_t numWalked = 0;

  Token* token = nullptr;

  for (const auto & it : this->tokenList) {
    if (numWalked == i) {
      token = it.get();
      break;
    }
    numWalked++;
  }

  return *token;
}

};