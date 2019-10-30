#ifndef TOKEN_BUFFER_HPP
#define TOKEN_BUFFER_HPP

#include "lib.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"

class TokenBuffer {
public:
  std::list<std::shared_ptr<Token>> tokenList;
  const std::shared_ptr<Tokenizer> tokenizer;

  explicit TokenBuffer(std::shared_ptr<Tokenizer> tokenizer) noexcept
  : tokenizer{std::move(tokenizer)}
  {}

  const std::shared_ptr<Token> tokenAt(std::size_t i) noexcept;

  const std::shared_ptr<Token> currentToken() noexcept;

  const std::shared_ptr<Token> nextToken() noexcept;

  void advance() noexcept;
};

#endif
