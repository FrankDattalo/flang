#include "Tokenizer.hpp"

class TokenizerRule {
public:
  virtual std::pair<bool, std::size_t> tryMatch(std::shared_ptr<Reader> reader) noexcept = 0;

  virtual TokenType getType() noexcept = 0;
};

class LiteralTokenizerRule : public TokenizerRule {
private:
  const TokenType type;
  const std::string literal;

public:
  explicit LiteralTokenizerRule(TokenType type, std::string literal) noexcept
  : type{type}, literal{std::move(literal)}
  {}

  virtual ~LiteralTokenizerRule() = default;

  std::pair<bool, std::size_t> tryMatch(std::shared_ptr<Reader> reader) noexcept override {
    for (std::size_t i = 0; i < this->literal.length(); i++) {
      if (this->literal.at(i) != reader->charAt(i)) {
        return std::make_pair<bool, std::size_t>(false, 0);
      }
    }
    return std::make_pair<bool, std::size_t>(true, this->literal.length());
  }

  TokenType getType() noexcept override {
    return this->type;
  }
};

const std::vector<std::shared_ptr<TokenizerRule>> rules = {
  std::make_shared<LiteralTokenizerRule>(TokenType::LeftCurly, "{"),
  std::make_shared<LiteralTokenizerRule>(TokenType::RightCurly, "}"),
  std::make_shared<LiteralTokenizerRule>(TokenType::NewLine, "\r\n"),
  std::make_shared<LiteralTokenizerRule>(TokenType::NewLine, "\r"),
  std::make_shared<LiteralTokenizerRule>(TokenType::NewLine, "\n"),
};

std::shared_ptr<Token> Tokenizer::nextToken() noexcept {

  if (this->reader->charAt(0) == '\0') {
    return std::make_shared<Token>("EOF", TokenType::EndOfFile, 0, 0, 0);
  }

  bool match = false;
  std::size_t matchSize = 0;
  TokenType matchTokenType = TokenType::Unknown;

  for (auto & rule : rules) {
    auto result = rule->tryMatch(this->reader);

    match = result.first;
    matchSize = result.second;
    matchTokenType = rule->getType();

    if (match) { break; }
  }

  if (match && matchSize > 0 && matchTokenType != TokenType::Unknown) {

    this->token = std::make_shared<Token>(
      this->reader->subString(matchSize), matchTokenType, 
      this->sourceIndex, this->sourceLine, this->sourceColumn);

    if (matchTokenType == TokenType::NewLine) {
      this->sourceColumn = 1;
      this->sourceLine += 1;
    } else {
      this->sourceColumn += matchSize;
    }

    this->reader->advance(matchSize);

  } else {

    this->token = std::make_shared<Token>(
      std::string{this->reader->charAt(0)}, TokenType::Unknown, 
      this->sourceIndex, this->sourceLine, this->sourceColumn);

    this->sourceIndex++;
    this->sourceColumn++;

    this->reader->advance(1);
  }

  return this->token;
}

std::shared_ptr<Token> Tokenizer::currentToken() const noexcept {
  return this->token;
} 

const std::string Tokenizer::toString() const noexcept {
  return (
    "Token(" 
      "reader: " + this->reader->toString() +
      ", sourceIndex: " + std::to_string(this->sourceIndex) +
      ", sourceLine: " + std::to_string(this->sourceLine) +
      ", sourceColumn " + std::to_string(this->sourceColumn) +
    ")"
  );
}