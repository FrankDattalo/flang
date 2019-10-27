#include "SemanticAnalyzer.hpp"

class Scope {
private:
  const std::optional<std::shared_ptr<Scope>> outerScope;
  std::map<std::string, std::shared_ptr<Token>> localScope;

public:
  explicit Scope() noexcept
  : outerScope{std::nullopt}
  {}

  explicit Scope(std::shared_ptr<Scope> outerScope) noexcept
  : outerScope{outerScope}
  {}

  ~Scope() = default;

  bool hasOuterScope() const {
    return this->outerScope.has_value();
  }

  void define(const std::string & variableName, std::shared_ptr<Token> token) {
    this->localScope.insert(std::make_pair(variableName, token));
  }

  std::optional<std::shared_ptr<Token>> findLocally(const std::string & variableName) {
    auto find = this->localScope.find(variableName);

    if (find == this->localScope.end()) {
      return std::nullopt;
    }

    return find->second;
  }

  std::optional<std::shared_ptr<Token>> find(const std::string & variableName) {

    Scope* scopeToSearch = this;

    while (true) {

      auto find = scopeToSearch->findLocally(variableName);

      if (find) {
        return find;
      }

      if (scopeToSearch->outerScope) {
        scopeToSearch = outerScope.value().get();

      } else {
        return std::nullopt;
      }
    }
  }

  std::optional<std::shared_ptr<Scope>> getOuterScope() {
    return this->outerScope;
  }
};

class SemanticAnalyzerRun : public AstWalker {
private:
  std::ostream & out;
  const std::shared_ptr<const Readable> reader;
  bool error;
  // std::size_t functionDept;
  // bool inLoop;
  std::map<FunctionDeclarationExpressionAstNode*, bool> inLoopState;
  std::shared_ptr<Scope> currentScope;

public:
  explicit SemanticAnalyzerRun(std::ostream & out, std::shared_ptr<const Readable> reader) noexcept
  : out{out}
  , reader{std::move(reader)}
  , error{false}
  // , functionDept{0}
  // , inLoop{false}
  , currentScope{std::make_shared<Scope>()}
  {}

  ~SemanticAnalyzerRun() noexcept override = default;

  bool hasErrors() noexcept {
    return this->error;
  }

  // validate that the variable we are delcaring has not already been declared elsewhere
  void onEnterDeclareStatementAstNode(DeclareStatementAstNode*  /*node*/) noexcept override {

  }

  // validate that the variable we are assign has been declared within the current scope
  void onEnterAssignStatementAstNode(AssignStatementAstNode*  /*node*/) noexcept override {

  }

  // validate that we're in a loop
  void onEnterBreakStatementAstNode(BreakStatementAstNode*  /*node*/) noexcept override {

  }

  // validate that function dept is not zero
  void onEnterReturnStatementAstNode(ReturnStatementAstNode*  /*node*/) noexcept override {

  }

  // increase scope
  void onEnterBlockStatementAstNode(BlockStatementAstNode*  /*node*/) noexcept override {

  }

  // mark in loop to true
  void onEnterWhileStatementAstNode(WhileStatementAstNode*   /*node*/) noexcept override {

  }

  // validate that identifier is defined
  void onEnterIdentifierExpressionAstNode(IdentifierExpressionAstNode*  /*node*/) noexcept override {

  }

  // validate that identifier is defined
  void onEnterFunctionInvocationExpressionAstNode(FunctionInvocationExpressionAstNode*  /*node*/) noexcept override {

  }

  // increase scope, define arguments
  void onEnterFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode*  /*node*/) noexcept override {

  }

  // validate that the numeric literals can be constructed properly
  void onEnterLiteralExpressionAstNode(LiteralExpressionAstNode* node) noexcept override {
    switch (node->token->tokenType) {
      case TokenType::IntegerLiteral: {
        try {
          std::stoll(node->token->value);
        } catch (...) {
          this->reportError(node->token, "Invalid value for integer literal.");
        }
      }
      case TokenType::FloatLiteral: {
        try {
          std::stod(node->token->value);
        } catch (...) {
          this->reportError(node->token, "Invalid value for float literal.");
        }
      }
      default:
        return;
    }
  }

  // define the variable within the current scope
  void onExitDeclareStatementAstNode(DeclareStatementAstNode*  /*node*/) noexcept override {

  }

  // pop scope
  void onExitBlockStatementAstNode(BlockStatementAstNode*  /*node*/) noexcept override {

  }

  // mark in loop to false
  void onExitWhileStatementAstNode(WhileStatementAstNode*   /*node*/) noexcept override {

  }

  // pop scope
  void onExitFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode*  /*node*/) noexcept override {

  }

  void reportError(const std::shared_ptr<Token>& token, const std::string & errorMessage) noexcept {
    this->error = true;
    ErrorUtils::reportErrorAtToken(this->out, "semantic analysis", this->reader, token, errorMessage);
  }
};

bool SemanticAnalyzer::isValid(const std::shared_ptr<ScriptAstNode>& script) noexcept {

  SemanticAnalyzerRun run{this->out, this->reader};

  run.visitScriptAstNode(script.get());

  return !run.hasErrors();
}