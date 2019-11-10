#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "utils/lib.hpp"
#include "frontend/Token.hpp"

namespace frontend {

class Tokenizer {
private:
  const std::string_view source;
  std::size_t sourceIndex;
  std::size_t sourceLine;
  std::size_t sourceColumn;

  NOT_COPYABLE(Tokenizer);

public:
  explicit Tokenizer(const std::string_view) noexcept;

  std::unique_ptr<Token> NextToken() noexcept;

  void ReportErrorAtToken(std::ostream & out, const std::string_view phase, const Token& token, const std::string & message);

private:
  const std::string_view LineForToken(const Token&) noexcept;
};

};

#endif // TOKENIZER_HPP