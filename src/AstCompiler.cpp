#include "AstCompiler.hpp"

namespace compiler {

class EmissionContext {
public:
  // TODO: make this faster? O(n) to find the proper variable
  std::vector<std::string> variables;
  std::size_t firstFreeVariablesIndex{0};
  std::vector<std::size_t> scopeStartIndex{0};
  std::vector<bytecode::ByteCode> byteCode;
  std::shared_ptr<EmissionContext> outerContext;

  std::vector<std::size_t> loopStartIndices;
  std::vector<std::size_t> loopConditionEvalJumpIndices;
  std::unordered_map<std::size_t, std::shared_ptr<std::vector<std::size_t>>> breakStatementsForLoops;

  std::vector<std::size_t> ifConditionEvalJumpIndices;
  std::vector<std::size_t> elseStatementStartIndex;

  explicit EmissionContext(
    std::shared_ptr<EmissionContext> outerContext
  ) noexcept
  : outerContext{std::move(outerContext)}
  {}

  std::size_t GetDeclarationIndex(const std::string & var) {

    for (std::size_t i = this->firstFreeVariablesIndex; i-- > 0;) {
      if (variables.at(i) == var) {
        return i;
      }
    }

    Error::assertWithPanic(false, "GetDeclarationIndex could not find declaration for provided variable");
    return 0;
  }

  void Declare(const std::string & var) {
    if (this->firstFreeVariablesIndex < this->variables.size()) {
      this->variables.at(this->firstFreeVariablesIndex) = var;
    } else {
      this->variables.push_back(var);
    }
    this->firstFreeVariablesIndex++;
  }

  void PushScope() {
    this->scopeStartIndex.push_back(this->firstFreeVariablesIndex);
  }

  void UpdateParameterAtIndex(std::size_t index, std::size_t parameter) {
    // TODO: fix this horribleness
    bytecode::ByteCode & bc = this->byteCode.at(index);
    std::size_t* parameterPtr = const_cast<std::size_t*>(&bc.parameter);
    *parameterPtr = parameter;
  }

  void PopScope() {
    Error::assertWithPanic(!this->scopeStartIndex.empty(), "PopScope called with empty scopeStartIndex");

    this->firstFreeVariablesIndex = this->scopeStartIndex.at(this->scopeStartIndex.size() - 1);
    this->scopeStartIndex.pop_back();
  }

  void EmitByteCode(bytecode::ByteCode bc) {
    this->byteCode.push_back(bc);
  }
};

class CompilerAstWalker : public AstWalker {
public:
    std::vector<bytecode::Function> functions;
    std::vector<bytecode::ObjectConstructor> objects;

    std::vector<std::int64_t> intConstants;
    std::unordered_map<std::int64_t, std::size_t> intConstantsLookup;

    std::vector<double> floatConstants;
    std::unordered_map<double, std::size_t> floatConstantsLookup;

    std::vector<std::string> stringConstants;
    std::unordered_map<std::string, std::size_t> stringConstantsLookup;

    std::shared_ptr<compiler::EmissionContext> ec;

    std::unordered_map<std::string, bytecode::ByteCodeInstruction> builtInFunctionLookup{{
      {"add",            bytecode::ByteCodeInstruction::Add},
      {"subtract",       bytecode::ByteCodeInstruction::Subtract},
      {"multiply",       bytecode::ByteCodeInstruction::Multiply},
      {"divide",         bytecode::ByteCodeInstruction::Divide},
      {"equal",          bytecode::ByteCodeInstruction::Equal},
      {"notEqual",       bytecode::ByteCodeInstruction::NotEqual},
      {"not",            bytecode::ByteCodeInstruction::Not},
      {"and",            bytecode::ByteCodeInstruction::And},
      {"or",             bytecode::ByteCodeInstruction::Or},
      {"greater",        bytecode::ByteCodeInstruction::Greater},
      {"less",           bytecode::ByteCodeInstruction::Less},
      {"greaterOrEqual", bytecode::ByteCodeInstruction::GreaterOrEqual},
      {"lessOrEqual",    bytecode::ByteCodeInstruction::LessOrEqual},
      {"get",            bytecode::ByteCodeInstruction::ObjectGet},
      {"set",            bytecode::ByteCodeInstruction::ObjectSet},
      {"read",           bytecode::ByteCodeInstruction::Read},
      {"print",          bytecode::ByteCodeInstruction::Print},
      {"env",            bytecode::ByteCodeInstruction::GetEnv},
      {"type",           bytecode::ByteCodeInstruction::GetType},
      {"int",            bytecode::ByteCodeInstruction::CastToInt},
      {"float",          bytecode::ByteCodeInstruction::CastToFloat},
      {"length",         bytecode::ByteCodeInstruction::Length},
      {"charAt",         bytecode::ByteCodeInstruction::ChatAt},
      {"append",         bytecode::ByteCodeInstruction::StringAppend}
    }};

  explicit CompilerAstWalker() noexcept
  : ec{std::make_shared<EmissionContext>(nullptr)}
  {}

  virtual ~CompilerAstWalker() noexcept = default;

  std::shared_ptr<bytecode::CompiledFile> ConstructCompiledFile() noexcept {
    return std::make_shared<bytecode::CompiledFile>(
      bytecode::Function{ 0, this->ec->variables.size(), 0, this->ec->byteCode },
      this->functions,
      this->objects,
      this->intConstants,
      this->floatConstants,
      this->stringConstants
    );
  }

  template<typename T>
  std::size_t indexOfConstant(T t, std::vector<T> & vec, std::unordered_map<T, std::size_t> & lookup) noexcept {
    auto find = lookup.find(t);

    if (find == lookup.end()) {
      std::size_t vecLen = vec.size();
      vec.push_back(t);
      lookup.insert(std::make_pair(t, vecLen));
      return vecLen;

    } else {
      return find->second;
    }
  }

  void emit(bytecode::ByteCodeInstruction instruction) noexcept {
    this->emit(instruction, 0);
  }

  void emit(bytecode::ByteCodeInstruction instruction, std::size_t arg) noexcept {
    this->ec->EmitByteCode(bytecode::ByteCode{instruction, arg});
  }

  void popEmissionContext() noexcept {
    auto outer = this->ec->outerContext;
    Error::assertWithPanic(outer != nullptr, "popEmissionContext called when outer context is none!");
    this->ec = outer;
  }

  void onEnterWhileStatementAstNode(WhileStatementAstNode*  /*node*/) noexcept override {
    this->ec->loopStartIndices.push_back(this->ec->byteCode.size());
  }

  void onEnterBreakStatementAstNode(BreakStatementAstNode*  /*node*/) noexcept override {
    Error::assertWithPanic(!this->ec->loopStartIndices.empty(),
      "onEnterBreakStatementAstNode encountered break statement but loopStartIndices empty");

    std::size_t breakIndex = this->ec->byteCode.size();
    std::size_t loopIndex = this->ec->loopStartIndices.at(this->ec->loopStartIndices.size() - 1);

    auto find = this->ec->breakStatementsForLoops.find(loopIndex);

    std::shared_ptr<std::vector<std::size_t>> vec = nullptr;

    if (find == this->ec->breakStatementsForLoops.end()) {
      vec = std::make_shared<std::vector<std::size_t>>();
      this->ec->breakStatementsForLoops.insert(std::make_pair(loopIndex, vec));
    } else {
      vec = find->second;
    }

    vec->push_back(breakIndex);

    this->emit(bytecode::ByteCodeInstruction::Jump);
  }

  void visitWhileStatementAstNode(WhileStatementAstNode* node) noexcept override {
    this->onEnterWhileStatementAstNode(node);
    this->visitExpressionAstNode(node->expression.get());
    this->ec->loopConditionEvalJumpIndices.push_back(this->ec->byteCode.size());
    this->emit(bytecode::ByteCodeInstruction::JumpIfFalse);
    this->visitStatementAstNode(node->statement.get());
    this->onExitWhileStatementAstNode(node);
  }

  void visitIfStatementAstNode(IfStatementAstNode* node) noexcept override {
    this->onEnterIfStatementAstNode(node);
    this->visitExpressionAstNode(node->expression.get());
    this->ec->ifConditionEvalJumpIndices.push_back(this->ec->byteCode.size());
    this->emit(bytecode::ByteCodeInstruction::JumpIfFalse);
    this->visitStatementAstNode(node->ifStatement.get());
    if (node->elseStatement) {
      this->emit(bytecode::ByteCodeInstruction::Jump);
      this->ec->elseStatementStartIndex.push_back(this->ec->byteCode.size());
      this->visitStatementAstNode(node->elseStatement->get());
    }
    this->onExitIfStatementAstNode(node);
  }

  void onEnterBlockStatementAstNode(BlockStatementAstNode*  /*node*/) noexcept override {
    this->ec->PushScope();
  }

  void onEnterLiteralExpressionAstNode(LiteralExpressionAstNode* node) noexcept override {
    switch (node->token->tokenType) {
      case TokenType::BooleanLiteral: {
        if (node->token->value == "true") {
          this->emit(bytecode::ByteCodeInstruction::LoadBooleanTrueConstant);
        } else if (node->token->value == "false") {
          this->emit(bytecode::ByteCodeInstruction::LoadBooleanFalseConstant);
        } else {
          Error::assertWithPanic(false, "Encountered unknown boolean literal value in AstCompiler.cpp onEnterLiteralExpressionAstNode");
        }
        return;
      }
      case TokenType::FloatLiteral: {
        double val = std::stod(node->token->value);
        std::size_t index = this->indexOfConstant(val, this->floatConstants, this->floatConstantsLookup);
        this->emit(bytecode::ByteCodeInstruction::LoadFloatConstant, index);
        return;
      }
      case TokenType::IntegerLiteral: {
        std::int64_t val = std::stoll(node->token->value);
        std::size_t index = this->indexOfConstant(val, this->intConstants, this->intConstantsLookup);
        this->emit(bytecode::ByteCodeInstruction::LoadIntegerConstant, index);
        return;
      }
      case TokenType::UndefinedLiteral: {
        this->emit(bytecode::ByteCodeInstruction::LoadUndefinedConstant);
        return;
      }
      case TokenType::StringLiteral: {
        auto str = node->token->value.substr(1, node->token->value.length() - 2);
        for (std::size_t i = 0; i < str.length(); i++) {
          if (str.at(i) == '\\' && i < str.length() - 1 && str.at(i + 1) == 'n') {
            str = str.replace(i, 2, "\n");
          }
        }
        std::size_t index = this->indexOfConstant(str, this->stringConstants, this->stringConstantsLookup);
        this->emit(bytecode::ByteCodeInstruction::LoadStringConstant, index);
        return;
      }
      default: {
        Error::assertWithPanic(false, "Encountered unknown literal type in AstCompiler.cpp onEnterLiteralExpressionAstNode");
        return;
      }
    }
  }

  void onEnterIdentifierExpressionAstNode(IdentifierExpressionAstNode* node) noexcept override {
    std::size_t index = this->ec->GetDeclarationIndex(node->token->value);
    this->emit(bytecode::ByteCodeInstruction::LoadLocal, index);
  }

  void onEnterFunctionInvocationExpressionAstNode(FunctionInvocationExpressionAstNode* node) noexcept override {
    std::size_t index = this->ec->GetDeclarationIndex(node->identifier->value);
    this->emit(bytecode::ByteCodeInstruction::LoadLocal, index);
  }

  void onEnterFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode* node) noexcept override {
    auto ec = std::make_shared<compiler::EmissionContext>(this->ec);

    for (const auto& var : node->parameters) {
      ec->Declare(var->value);
    }

    this->ec = ec;
  }

  // exit callbacks
  void onExitScriptAstNode(ScriptAstNode*  /*node*/) noexcept override {
   this->emit(bytecode::ByteCodeInstruction::Halt);
  }

  void onExitDeclareStatementAstNode(DeclareStatementAstNode* node) noexcept override {
    this->ec->Declare(node->identifier->value);
    std::size_t index = this->ec->GetDeclarationIndex(node->identifier->value);
    this->emit(bytecode::ByteCodeInstruction::SetLocal, index);
  }

  void onExitAssignStatementAstNode(AssignStatementAstNode* node) noexcept override {
    std::size_t index = this->ec->GetDeclarationIndex(node->identifier->value);
    this->emit(bytecode::ByteCodeInstruction::SetLocal, index);
  }

  void onExitIfStatementAstNode(IfStatementAstNode* node) noexcept override {
    Error::assertWithPanic(!this->ec->ifConditionEvalJumpIndices.empty(),
      "onExitIfStatementAstNode encountered if exit statement but ifConditionEvalJumpIndices is empty");

    std::size_t ifEvalIndex = this->ec->ifConditionEvalJumpIndices.at(this->ec->ifConditionEvalJumpIndices.size() - 1);
    this->ec->ifConditionEvalJumpIndices.pop_back();

    std::size_t jumpIndex;

    if (node->elseStatement) {
      Error::assertWithPanic(!this->ec->elseStatementStartIndex.empty(),
        "onExitIfStatementAstNode encountered if exit statement with valid ekse but elseStatementStartIndex is empty");

      jumpIndex = this->ec->elseStatementStartIndex.at(this->ec->elseStatementStartIndex.size() - 1);

      Error::assertWithPanic(jumpIndex > 0,
        "onExitIfStatementAstNode encountered jumpIndex == 0 for after if statement -> jump past else");

      this->ec->UpdateParameterAtIndex(jumpIndex - 1, this->ec->byteCode.size());

      this->ec->elseStatementStartIndex.pop_back();

    } else {
      jumpIndex = this->ec->byteCode.size();
    }

    this->ec->UpdateParameterAtIndex(ifEvalIndex, jumpIndex);
  }

  void onExitWhileStatementAstNode(WhileStatementAstNode*  /*node*/) noexcept override {
    Error::assertWithPanic(!this->ec->loopStartIndices.empty(),
      "onExitWhileStatementAstNode encountered while exit statement but loopStartIndices is empty");

    Error::assertWithPanic(!this->ec->loopConditionEvalJumpIndices.empty(),
      "onExitWhileStatementAstNode encountered while exit statement but loopConditionEvalJumpIndices is empty");

    std::size_t loopStartIndex = this->ec->loopStartIndices.at(this->ec->loopStartIndices.size() - 1);
    std::size_t loopJumpIfFalseIndex = this->ec->loopConditionEvalJumpIndices.at(this->ec->loopConditionEvalJumpIndices.size() - 1);

    auto find = this->ec->breakStatementsForLoops.find(loopStartIndex);

    std::shared_ptr<std::vector<std::size_t>> vec = nullptr;

    if (find != this->ec->breakStatementsForLoops.end()) {
      vec = find->second;
    }

    this->emit(bytecode::ByteCodeInstruction::Jump, loopStartIndex);

    std::size_t loopEnd = this->ec->byteCode.size();

    this->ec->UpdateParameterAtIndex(loopJumpIfFalseIndex, loopEnd);

    if (vec != nullptr) {
      for (const auto breakIndex : *vec) {
        this->ec->UpdateParameterAtIndex(breakIndex, loopEnd);
      }
    }

    this->ec->loopStartIndices.pop_back();
    this->ec->loopConditionEvalJumpIndices.pop_back();
  }

  void onExitReturnStatementAstNode(ReturnStatementAstNode* node) noexcept override {
    if (!node->expression) {
      this->emit(bytecode::ByteCodeInstruction::LoadUndefinedConstant);
    }
    this->emit(bytecode::ByteCodeInstruction::Return);
  }

  void onExitBlockStatementAstNode(BlockStatementAstNode* /*node*/) noexcept override {
    this->ec->PopScope();
  }

  void onExitFunctionInvocationExpressionAstNode(FunctionInvocationExpressionAstNode* node) noexcept override {
    this->emit(bytecode::ByteCodeInstruction::Invoke, node->expressions.size());
  }

  void onExitBuiltInFunctionInvocationExpressionAstNode(BuiltInFunctionInvocationExpressionAstNode* node) noexcept override {
    auto find = this->builtInFunctionLookup.find(node->identifier->value);

    Error::assertWithPanic(
      find != this->builtInFunctionLookup.end(),
      "onExitBuiltInFunctionInvocationExpressionAstNode found a built in function it does not know about");

    this->emit(find->second);
  }

  void onExitFunctionDeclarationExpressionAstNode(FunctionDeclarationExpressionAstNode* node) noexcept override {

    this->emit(bytecode::ByteCodeInstruction::LoadUndefinedConstant);
    this->emit(bytecode::ByteCodeInstruction::Return);

    std::size_t fnIndex = this->functions.size();

    this->functions.emplace_back(bytecode::Function{
      node->parameters.size(),
      this->ec->variables.size(),
      0,
      this->ec->byteCode
    });

    this->popEmissionContext();

    this->emit(bytecode::ByteCodeInstruction::MakeFn, fnIndex);
  }

  void onExitObjectDeclarationExpressionAstNode(ObjectDeclarationExpressionAstNode* node) noexcept override {

    std::vector<std::string> keys;
    keys.reserve(node->keyValues.size());

    for (const auto& kv : node->keyValues) {
      keys.push_back(kv.first->value);
    }

    std::size_t objectIndex = this->objects.size();

    this->objects.emplace_back(bytecode::ObjectConstructor{keys});

    this->emit(bytecode::ByteCodeInstruction::MakeObj, objectIndex);
  }

};

std::shared_ptr<bytecode::CompiledFile> compiler::AstCompiler::compile(std::shared_ptr<ScriptAstNode> file) noexcept {
  CompilerAstWalker astWalker;

  astWalker.visitScriptAstNode(file.get());

  return astWalker.ConstructCompiledFile();
}

};