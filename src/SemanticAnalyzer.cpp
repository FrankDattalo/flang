#include "SemanticAnalyzer.hpp"

class Scope {
public:
  const std::optional<std::shared_ptr<Scope>> outerScope;
  std::unordered_map<std::string, std::shared_ptr<Token>> localScope;

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
        scopeToSearch = scopeToSearch->outerScope.value().get();

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
public:
  std::ostream & out;
  const std::shared_ptr<const Readable> reader;

  // these are modified to check semantic corectness
  bool error;
  std::size_t functionDept;
  bool inLoop;
  std::unordered_map<FunctionDeclarationExpressionAstNode*, bool> inLoopState;
  std::shared_ptr<Scope> currentScope;

  explicit SemanticAnalyzerRun(std::ostream & out, std::shared_ptr<const Readable> reader) noexcept
  : out{out}
  , reader{std::move(reader)}
  , error{false}
  , functionDept{0}
  , inLoop{false}
  , currentScope{std::make_shared<Scope>()}
  {}

  ~SemanticAnalyzerRun() noexcept override = default;

  bool hasErrors() noexcept {
    return this->error;
  }

  // validate that the variable we are delcaring has not already been declared elsewhere
  void onEnterDeclareStatementAstNode(DeclareStatementAstNode* node) noexcept override {
    auto originalDeclare = this->currentScope->findLocally(node->identifier->value);
    if (originalDeclare != std::nullopt) {
      this->reportError(node->identifier, "Duplicate identifier found within same scope.");
    }
  }

  // validate that the variable we are assign has been declared within the current scope
  void onEnterAssignStatementAstNode(AssignStatementAstNode* node) noexcept override {
    auto originalDeclare = this->currentScope->findLocally(node->identifier->value);
    if (originalDeclare == std::nullopt) {
      this->reportError(node->identifier, "No local declaration found for identifier.");
    }
  }

  // validate that we're in a loop
  void onEnterBreakStatementAstNode(BreakStatementAstNode*  node) noexcept override {
    if (!this->inLoop) {
      this->reportError(node->breakToken, "Break statements can only be used from within loops.");
    }
  }

  // validate that function dept is not zero
  void onEnterReturnStatementAstNode(ReturnStatementAstNode*  node) noexcept override {
    if (functionDept == 0) {
      this->reportError(node->returnToken, "Return statements can only be used from within functions.");
    }
  }

  void pushScope(const std::vector<std::shared_ptr<Token>>& itemsToDeclare) noexcept {
    auto newScope = std::make_shared<Scope>(this->currentScope);
    this->currentScope = newScope;

    for (auto& item : itemsToDeclare) {
      auto find = this->currentScope->findLocally(item->value);

      if (find) {
        this->reportError(item, "Duplicate identifier found within the same scope.");

      } else {
        this->currentScope->define(item->value, item);
      }
    }
  }

  // increase scope
  void onEnterBlockStatementAstNode(BlockStatementAstNode* /*node*/) noexcept override {
    this->pushScope({});
  }

  // mark in loop to true
  void onEnterWhileStatementAstNode(WhileStatementAstNode* /*node*/) noexcept override {
    this->inLoop = true;
  }

  // validate that identifier is defined
  void onEnterIdentifierExpressionAstNode(IdentifierExpressionAstNode* node) noexcept override {
    if (!this->currentScope->find(node->token->value)) {
      this->reportError(node->token, "Undefined reference in identifier evaluation.");
    }
  }

  // validate that identifier is defined
  void onEnterFunctionInvocationExpressionAstNode(FunctionInvocationExpressionAstNode*  node) noexcept override {
    if (!this->currentScope->find(node->identifier->value)) {
      this->reportError(node->identifier, "Undefined reference in function invocation.");
    }
  }

  // increase scope, define arguments
  void onEnterFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode* node) noexcept override {
    this->functionDept++;
    this->inLoopState.insert(std::make_pair(node, this->inLoop));
    this->inLoop = false;
    this->pushScope(node->parameters);
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
  void onExitDeclareStatementAstNode(DeclareStatementAstNode* node) noexcept override {
    auto find = this->currentScope->findLocally(node->identifier->value);

    if (!find) {
      this->currentScope->define(node->identifier->value, node->identifier);
    }
  }

  void popScope() noexcept {
    auto outer = this->currentScope->getOuterScope();

    Error::assertWithPanic(outer != std::nullopt, "No outer scope found.");

    this->currentScope = outer.value();
  }

  // pop scope
  void onExitBlockStatementAstNode(BlockStatementAstNode* /*node*/) noexcept override {
    this->popScope();
  }

  // mark in loop to false
  void onExitWhileStatementAstNode(WhileStatementAstNode* /*node*/) noexcept override {
    this->inLoop = false;
  }

  // pop scope, decrease function dept, and restore loop setting
  void onExitFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode* node) noexcept override {
    Error::assertWithPanic(
      this->functionDept != 0,
      "Expected a non zero function dept when calling onExitFunctionDeclarationExpressionAstNode");

    this->functionDept--;

    auto search = this->inLoopState.find(node);

    Error::assertWithPanic(
      search != this->inLoopState.end(),
      "Could not find a key in inLoopState map for function declaration");

    this->inLoop = search->second;

    this->popScope();
  }

  void reportError(const std::shared_ptr<Token>& token, const std::string & errorMessage) noexcept {
    this->error = true;
    Error::reportErrorAtToken(this->out, "semantic analysis", this->reader, token, errorMessage);
  }
};

bool SemanticAnalyzer::isValid(const std::shared_ptr<ScriptAstNode>& script) noexcept {

  SemanticAnalyzerRun run{this->out, this->reader};

  run.visitScriptAstNode(script.get());

  return !run.hasErrors();
}