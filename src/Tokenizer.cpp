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
        return std::make_pair(false, 0);
      }
    }
    return std::make_pair(true, this->literal.length());
  }

  TokenType getType() noexcept override {
    return this->type;
  }
};

bool isNumeric(char c) {
  return '0' <= c && c <= '9';
}

bool isLowerAlpha(char c) {
  return 'a' <= c && c <= 'z';
}

bool isUpperAlpha(char c) {
  return 'A' <= c && c <= 'Z';
}

bool isLiteralFirst(char c) {
  return isLowerAlpha(c) || isUpperAlpha(c) || (c == '_');
}

bool isLiteralRest(char c) {
  return isLowerAlpha(c) || isUpperAlpha(c) || (c == '_') || isNumeric(c);
}

class IdentifierTokenizerRule : public TokenizerRule {
public:
  explicit IdentifierTokenizerRule() noexcept = default;

  virtual ~IdentifierTokenizerRule() = default;

  std::pair<bool, std::size_t> tryMatch(std::shared_ptr<Reader> reader) noexcept override {
    std::size_t i = 0;

    if (!isLiteralFirst(reader->charAt(i))) {
      return std::make_pair(false, 0);
    }

    i++;

    for (; reader->charAt(i) != '\0'; i++) {
      if (!isLiteralRest(reader->charAt(i))) {
        break;
      }
    }

    return std::make_pair(true, i);
  }

  TokenType getType() noexcept override {
    return TokenType::Identifier;
  }
};

bool isNewLine(const std::shared_ptr<Reader>& reader, std::size_t i) {
  return
    reader->charAt(i) == '\n'
    || reader->charAt(i) == '\r'
    || (reader->charAt(i) == '\r' && reader->charAt(i + 1) == '\n');
}

class StringTokenizerRule : public TokenizerRule {
public:
  explicit StringTokenizerRule() noexcept = default;

  virtual ~StringTokenizerRule() = default;

  std::pair<bool, std::size_t> tryMatch(std::shared_ptr<Reader> reader) noexcept override {
    std::size_t i = 0;

    if (reader->charAt(i) != '"') {
      return std::make_pair(false, 0);
    }

    i++;

    for (; reader->charAt(i) != '\0'; i++) {
      if (isNewLine(reader, i)) {
        return std::make_pair(false, 0);
      }
      if (reader->charAt(i) == '"') {
        // + 1 for the closing quote
        return std::make_pair(true, i + 1);
      }
    }

    return std::make_pair(false, 0);
  }

  TokenType getType() noexcept override {
    return TokenType::StringLiteral;
  }
};

class IntegerTokenizerRule : public TokenizerRule {
public:
  explicit IntegerTokenizerRule() noexcept = default;

  virtual ~IntegerTokenizerRule() = default;

  std::pair<bool, std::size_t> tryMatch(std::shared_ptr<Reader> reader) noexcept override {
    std::size_t i = 0;

    if (reader->charAt(i) == '0') {
      return std::make_pair(true, 1);
    }

    if ('1' <= reader->charAt(i) && reader->charAt(i) <= '9') {
      i++;
      while (isNumeric(reader->charAt(i))) {
        i++;
      }

      return std::make_pair(true, i);
    }

    return std::make_pair(false, 0);
  }

  TokenType getType() noexcept override {
    return TokenType::IntegerLiteral;
  }
};

class FloatTokenizerRule : public TokenizerRule {
public:
  explicit FloatTokenizerRule() noexcept = default;

  virtual ~FloatTokenizerRule() = default;

  std::pair<bool, std::size_t> handleFraction(const std::shared_ptr<Reader>& reader, std::size_t i) {
    if (reader->charAt(i) != '.') {
      return std::make_pair(false, 0);
    }

    i++;

    while (isNumeric(reader->charAt(i))) {
      i++;
    }

    return std::make_pair(true, i);
  }

  std::pair<bool, std::size_t> tryMatch(std::shared_ptr<Reader> reader) noexcept override {
    std::size_t i = 0;

    if (reader->charAt(i) == '0') {
      i++;
      return this->handleFraction(reader, i);
    }

    if ('1' <= reader->charAt(i) && reader->charAt(i) <= '9') {
      i++;
      while (isNumeric(reader->charAt(i))) {
        i++;
      }

      return this->handleFraction(reader, i);
    }

    return std::make_pair(false, 0);
  }

  TokenType getType() noexcept override {
    return TokenType::FloatLiteral;
  }
};

const std::vector<std::shared_ptr<TokenizerRule>> rules = {
  std::make_shared<LiteralTokenizerRule>(TokenType::LeftCurly, "{"),
  std::make_shared<LiteralTokenizerRule>(TokenType::RightCurly, "}"),
  std::make_shared<LiteralTokenizerRule>(TokenType::NewLine, "\r\n"),
  std::make_shared<LiteralTokenizerRule>(TokenType::NewLine, "\r"),
  std::make_shared<LiteralTokenizerRule>(TokenType::NewLine, "\n"),
  std::make_shared<LiteralTokenizerRule>(TokenType::WhiteSpace, "\t"),
  std::make_shared<LiteralTokenizerRule>(TokenType::WhiteSpace, " "),
  std::make_shared<LiteralTokenizerRule>(TokenType::LeftParen, "("),
  std::make_shared<LiteralTokenizerRule>(TokenType::RightParen, ")"),
  std::make_shared<LiteralTokenizerRule>(TokenType::LeftBracket, "["),
  std::make_shared<LiteralTokenizerRule>(TokenType::RightBracket, "]"),
  std::make_shared<LiteralTokenizerRule>(TokenType::Function, "function"),
  std::make_shared<LiteralTokenizerRule>(TokenType::Return, "return"),
  std::make_shared<LiteralTokenizerRule>(TokenType::For, "for"),
  std::make_shared<LiteralTokenizerRule>(TokenType::If, "if"),
  std::make_shared<LiteralTokenizerRule>(TokenType::Else, "else"),
  std::make_shared<LiteralTokenizerRule>(TokenType::While, "while"),
  std::make_shared<LiteralTokenizerRule>(TokenType::SemiColon, ";"),
  std::make_shared<LiteralTokenizerRule>(TokenType::Var, "var"),
  std::make_shared<LiteralTokenizerRule>(TokenType::Assign, "="),
  std::make_shared<LiteralTokenizerRule>(TokenType::Comma, ","),
  std::make_shared<LiteralTokenizerRule>(TokenType::BooleanLiteral, "true"),
  std::make_shared<LiteralTokenizerRule>(TokenType::BooleanLiteral, "false"),
  std::make_shared<LiteralTokenizerRule>(TokenType::UndefinedLiteral, "undefined"),
  std::make_shared<LiteralTokenizerRule>(TokenType::Period, "."),
  std::make_shared<LiteralTokenizerRule>(TokenType::Colon, ":"),
  std::make_shared<IdentifierTokenizerRule>(),
  std::make_shared<StringTokenizerRule>(),
  std::make_shared<FloatTokenizerRule>(),
  std::make_shared<IntegerTokenizerRule>(),
};

std::shared_ptr<Token> Tokenizer::nextToken() noexcept {

  if (this->reader->charAt(0) == '\0') {
    return std::make_shared<Token>("EndOfFile", TokenType::EndOfFile, 0, 0, 0);
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

    this->sourceIndex += matchSize;

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