#ifndef PARSER_HPP
#define PARSER_HPP

#include "lib.hpp"
#include "Stringable.hpp"
#include "Ast.hpp"
#include "Tokenizer.hpp"

class Parser : public Stringable {
private:
  const std::shared_ptr<Tokenizer> tokenizer;

public:
  explicit Parser(
    std::shared_ptr<Tokenizer> tokenizer
  ) noexcept
  : tokenizer{std::move(tokenizer)}
  {}

  ~Parser() noexcept override = default;

   const std::string toString() const noexcept override;
};

#endif
