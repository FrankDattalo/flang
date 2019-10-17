#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "lib.hpp"

#include "Reader.hpp"
#include "Token.hpp"
#include "Stringable.hpp"

class Tokenizer : Stringable {
private:
  const std::shared_ptr<Reader> reader;
  std::size_t sourceIndex;
  std::size_t sourceLine;
  std::size_t sourceColumn;
  std::shared_ptr<Token> token;
public:

  explicit Tokenizer(std::shared_ptr<Reader> reader) noexcept
  : reader{std::move(reader)}, sourceIndex{0}, sourceLine{1}, sourceColumn{1}, 
    token{std::make_shared<Token>("", TokenType::Unknown, 0, 0, 0)}
  {}

  virtual ~Tokenizer() = default;

  std::shared_ptr<Token> nextToken() noexcept;

  std::shared_ptr<Token> currentToken() const noexcept; 

  const std::string toString() const noexcept override;
};

#endif // TOKENIZER_HPP