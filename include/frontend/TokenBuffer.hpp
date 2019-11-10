#ifndef TOKEN_BUFFER_HPP
#define TOKEN_BUFFER_HPP

#include "utils/lib.hpp"
#include "frontend/Token.hpp"
#include "frontend/Tokenizer.hpp"

namespace frontend {

class TokenBuffer {
private:
  Tokenizer tokenizer;
  std::list<std::unique_ptr<Token>> tokenList;

  NOT_COPYABLE(TokenBuffer);

public:
  explicit TokenBuffer(std::string_view) noexcept;

  const Token& TokenAt(std::size_t) noexcept;

  std::unique_ptr<Token> Advance() noexcept;

  void ReportErrorAtToken(std::ostream & out, std::string_view phase, const Token& token, const std::string & message);
};

};

#endif
