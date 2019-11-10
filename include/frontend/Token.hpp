#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "utils/lib.hpp"
#include "utils/macros.hpp"
#include "frontend/TokenType.hpp"

namespace frontend {

class Token {
private:
  const std::string_view value;
  const TokenType        tokenType;
  const std::size_t      sourceIndex;
  const std::size_t      sourceLine;
  const std::size_t      sourceColumn;

  NOT_COPYABLE(Token);

public:
  explicit Token(const std::string_view, TokenType, std::size_t, std::size_t, std::size_t) noexcept;

  HAS_GETTER(std::string_view, Value);

  HAS_GETTER(TokenType, TokenType);

  HAS_GETTER(std::size_t, SourceIndex);

  HAS_GETTER(std::size_t, SourceLine);

  HAS_GETTER(std::size_t, SourceColumn);
};

};

#endif