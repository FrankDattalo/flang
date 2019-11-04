#include "Tokenizer.hpp"

class TokenizerRule {
public:
  virtual std::pair<bool, std::size_t> tryMatch(std::shared_ptr<Readable> reader) noexcept = 0;

  virtual TokenType getType() noexcept = 0;
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

class LiteralTokenizerRule : public TokenizerRule {
public:
  const TokenType type;
  const std::string literal;
  const bool nextCharMustBeNonAlpha;

  explicit LiteralTokenizerRule(TokenType type, std::string literal, bool nextCharMustBeNonAlpha) noexcept
  : type{type}
  , literal{std::move(literal)}
  , nextCharMustBeNonAlpha{nextCharMustBeNonAlpha}
  {}

  virtual ~LiteralTokenizerRule() = default;

  std::pair<bool, std::size_t> tryMatch(std::shared_ptr<Readable> reader) noexcept override {
    for (std::size_t i = 0; i < this->literal.length(); i++) {
      if (this->literal.at(i) != reader->charAt(i)) {
        return std::make_pair(false, 0);
      }
    }

    if (this->nextCharMustBeNonAlpha && isLiteralRest(reader->charAt(this->literal.size()))) {
      return std::make_pair(false, 0);
    }

    return std::make_pair(true, this->literal.length());
  }

  TokenType getType() noexcept override {
    return this->type;
  }
};

class IdentifierTokenizerRule : public TokenizerRule {
public:
  explicit IdentifierTokenizerRule() noexcept = default;

  virtual ~IdentifierTokenizerRule() = default;

  std::pair<bool, std::size_t> tryMatch(std::shared_ptr<Readable> reader) noexcept override {
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

bool isNewLine(const std::shared_ptr<Readable>& reader, std::size_t i) {
  return
    reader->charAt(i) == '\n'
    || reader->charAt(i) == '\r'
    || (reader->charAt(i) == '\r' && reader->charAt(i + 1) == '\n');
}

class StringTokenizerRule : public TokenizerRule {
public:
  explicit StringTokenizerRule() noexcept = default;

  virtual ~StringTokenizerRule() = default;

  std::pair<bool, std::size_t> tryMatch(std::shared_ptr<Readable> reader) noexcept override {
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

  std::pair<bool, std::size_t> tryMatch(std::shared_ptr<Readable> reader) noexcept override {
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

  std::pair<bool, std::size_t> handleFraction(
    const std::shared_ptr<Readable>& reader,
    std::size_t i) {

    if (reader->charAt(i) != '.') {
      return std::make_pair(false, 0);
    }

    i++;

    while (isNumeric(reader->charAt(i))) {
      i++;
    }

    return std::make_pair(true, i);
  }

  std::pair<bool, std::size_t> tryMatch(std::shared_ptr<Readable> reader) noexcept override {
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

class CommentTokenizerRule : public TokenizerRule {
public:
  explicit CommentTokenizerRule() noexcept = default;

  virtual ~CommentTokenizerRule() = default;

  std::pair<bool, std::size_t> tryMatch(std::shared_ptr<Readable> reader) noexcept override {
    std::size_t i = 0;

    if (reader->charAt(i) != '#') {
      return std::make_pair(false, 0);
    }

    i++;

    while (reader->charAt(i) != '\0' && reader->charAt(i) != '\n' && reader->charAt(i) != '\r') {
      i++;
    }

    return std::make_pair(true, i);
  }

  TokenType getType() noexcept override {
    return TokenType::WhiteSpace;
  }
};

#define builtInFn(x) std::make_shared<LiteralTokenizerRule>(TokenType::BuiltInFunctionName, x, true)

const std::vector<std::shared_ptr<TokenizerRule>> rules = {
  std::make_shared<LiteralTokenizerRule>(TokenType::LeftCurly, "{", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::RightCurly, "}", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::WhiteSpace, "\r\n", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::WhiteSpace, "\r", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::WhiteSpace, "\n", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::WhiteSpace, "\t", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::WhiteSpace, " ", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::LeftParen, "(", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::RightParen, ")", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::LeftBracket, "[", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::RightBracket, "]", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::Function, "function", true),
  std::make_shared<LiteralTokenizerRule>(TokenType::Return, "return", true),
  std::make_shared<LiteralTokenizerRule>(TokenType::Break, "break", true),
  std::make_shared<LiteralTokenizerRule>(TokenType::For, "for", true),
  std::make_shared<LiteralTokenizerRule>(TokenType::If, "if", true),
  std::make_shared<LiteralTokenizerRule>(TokenType::Else, "else", true),
  std::make_shared<LiteralTokenizerRule>(TokenType::While, "while", true),
  std::make_shared<LiteralTokenizerRule>(TokenType::SemiColon, ";", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::Var, "var", true),
  std::make_shared<LiteralTokenizerRule>(TokenType::Assign, "=", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::Comma, ",", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::BooleanLiteral, "true", true),
  std::make_shared<LiteralTokenizerRule>(TokenType::BooleanLiteral, "false", true),
  std::make_shared<LiteralTokenizerRule>(TokenType::UndefinedLiteral, "undefined", true),
  std::make_shared<LiteralTokenizerRule>(TokenType::Period, ".", false),
  std::make_shared<LiteralTokenizerRule>(TokenType::Colon, ":", false),

  // math ops
  builtInFn("add"),
  builtInFn("subtract"),
  builtInFn("multiply"),
  builtInFn("divide"),

  // comparisons
  builtInFn("equal"),
  builtInFn("notEqual"),

  // boolean operations
  builtInFn("not"),
  builtInFn("and"),
  builtInFn("or"),

  // numeric comparisons
  builtInFn("greaterOrEqual"),
  builtInFn("lessOrEqual"),
  builtInFn("greater"),
  builtInFn("less"),

  // object manipulation
  builtInFn("get"),
  builtInFn("set"),

  // io
  builtInFn("read"),
  builtInFn("print"),
  builtInFn("env"),

  // run time type manipulation
  builtInFn("type"),
  builtInFn("int"),
  builtInFn("float"),

  // string manipulation
  builtInFn("length"),
  builtInFn("charAt"),
  builtInFn("append"),

  std::make_shared<IdentifierTokenizerRule>(),
  std::make_shared<StringTokenizerRule>(),
  std::make_shared<FloatTokenizerRule>(),
  std::make_shared<IntegerTokenizerRule>(),
  std::make_shared<CommentTokenizerRule>(),
};

#undef builtInFn

std::shared_ptr<Token> Tokenizer::nextToken() noexcept {

  if (this->reader->charAt(0) == '\0') {
    this->token = std::make_shared<Token>("EndOfFile", TokenType::EndOfFile, 0, 0, 0);
    return this->token;
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

    std::string str = this->reader->subString(matchSize);

    this->token = std::make_shared<Token>(
      str, matchTokenType, this->sourceIndex, this->sourceLine, this->sourceColumn);

    if (Token::isNewLine(str)) {
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