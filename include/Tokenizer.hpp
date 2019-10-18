#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "lib.hpp"

#include "Readable.hpp"
#include "Token.hpp"
#include "Stringable.hpp"

class Tokenizer : public Stringable {
private:
  const std::shared_ptr<Readable> reader;
  std::size_t sourceIndex;
  std::size_t sourceLine;
  std::size_t sourceColumn;
  std::shared_ptr<Token> token;
public:

  explicit Tokenizer(std::shared_ptr<Readable> reader) noexcept
  : reader{std::move(reader)},
    sourceIndex{0},
    sourceLine{1},
    sourceColumn{1},
    token{std::make_shared<Token>("Unknown", TokenType::Unknown, 0, 0, 0)}
  {}

  ~Tokenizer() override = default;

  std::shared_ptr<Token> nextToken() noexcept;

  std::shared_ptr<Token> currentToken() const noexcept;

  const std::string toString() const noexcept override;
};

#endif // TOKENIZER_HPP