#ifndef TOKEN_BUFFER_HPP
#define TOKEN_BUFFER_HPP

#include "utils/lib.hpp"
#include "frontend/Token.hpp"
#include "frontend/Tokenizer.hpp"

namespace frontend {

class TokenBuffer {
private:
  Tokenizer tokenizer;
  std::vector<std::unique_ptr<Token>> tokenList;
  std::size_t firstUsedIndex;

  NOT_COPYABLE(TokenBuffer);

public:
  explicit TokenBuffer(std::string_view) noexcept;

  const Token& TokenAt(std::size_t) noexcept;

  std::unique_ptr<Token> Advance() noexcept;
};

};

#endif
