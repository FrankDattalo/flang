#include "Runtime.hpp"

namespace runtime {

void ni() {
  std::cerr << "NOT IMPLEMENTED" << std::endl;
  exit(1);
}

enum class VariableType {
  Undefined,
  Integer,
  Boolean,
  Float,
  String,
  Object,
  Function,
};

struct Object;

struct Function {
  const bytecode::Function* fn;
};

struct Variable {
  VariableType type;

  union {
    std::int64_t integerValue;
    double doubleValue;
    bool boolValue;
    Object* objectValue;
    Function* functionValue;
    const std::string* stringValue;
  };
};

struct Object {
  std::unordered_map<std::string, Variable> properties;
};

struct StackFrame {
  std::size_t programCounter;
  std::vector<Variable> locals;
  const std::vector<bytecode::ByteCode>* byteCode;
  std::optional<std::shared_ptr<StackFrame>> outer;
  std::vector<Variable> opStack;
};

void runtime::VirtualMachine::run() noexcept {

  this->pushStackFrame(&this->file->entrypoint);

  this->heap.StartGc();

  while (true) {
    if (this->isDebug) {
      this->out << "BEGIN DEBUG\n";
      this->print();
      this->out << "END DEBUG\n";
    }

    if (this->stackFrame->programCounter >= this->stackFrame->byteCode->size()) {
      this->panic("Program counter overran bytecode!");
      return;
    }

    auto instruction = this->stackFrame->byteCode->at(this->stackFrame->programCounter).instruction;

    switch (instruction) {
      case bytecode::ByteCodeInstruction::Halt: { this->heap.EndGc(); return; }
      case bytecode::ByteCodeInstruction::Add: { this->Add(); break; }
      case bytecode::ByteCodeInstruction::Subtract: { this->Subtract(); break; }
      case bytecode::ByteCodeInstruction::Multiply: { this->Multiply(); break; }
      case bytecode::ByteCodeInstruction::Divide: { this->Divide(); break; }
      case bytecode::ByteCodeInstruction::Print: { this->Print(); break; }
      case bytecode::ByteCodeInstruction::Read: { this->Read(); break; }
      case bytecode::ByteCodeInstruction::Jump: { this->Jump(); break; }
      case bytecode::ByteCodeInstruction::JumpIfFalse: { this->JumpIfFalse(); break; }
      case bytecode::ByteCodeInstruction::LoadIntegerConstant: { this->LoadIntegerConstant(); break; }
      case bytecode::ByteCodeInstruction::LoadFloatConstant: { this->LoadFloatConstant(); break; }
      case bytecode::ByteCodeInstruction::LoadStringConstant: { this->LoadStringConstant(); break; }
      case bytecode::ByteCodeInstruction::LoadUndefinedConstant: { this->LoadUndefinedConstant(); break; }
      case bytecode::ByteCodeInstruction::LoadBooleanTrueConstant: { this->LoadBooleanTrueConstant(); break; }
      case bytecode::ByteCodeInstruction::LoadBooleanFalseConstant: { this->LoadBooleanFalseConstant(); break; }
      case bytecode::ByteCodeInstruction::LoadLocal: { this->LoadLocal(); break; }
      case bytecode::ByteCodeInstruction::SetLocal: { this->SetLocal(); break; }
      case bytecode::ByteCodeInstruction::Return: { this->Return(); break; }
      case bytecode::ByteCodeInstruction::Invoke: { this->Invoke(); break; }
      case bytecode::ByteCodeInstruction::NoOp: { break; }
      case bytecode::ByteCodeInstruction::MakeFn: { this->MakeFn(); break; }
      case bytecode::ByteCodeInstruction::MakeObj: { this->MakeObj(); break; }
      case bytecode::ByteCodeInstruction::Less: { this->Less(); break; }
      case bytecode::ByteCodeInstruction::LessOrEqual: { this->LessOrEqual(); break; }
      case bytecode::ByteCodeInstruction::Greater: { this->Greater(); break; }
      case bytecode::ByteCodeInstruction::GreaterOrEqual: { this->GreaterOrEqual(); break; }
      case bytecode::ByteCodeInstruction::Not: { this->Not(); break; }
      case bytecode::ByteCodeInstruction::Equal: { this->Equal(); break; }
      case bytecode::ByteCodeInstruction::NotEqual: { this->NotEqual(); break; }
      case bytecode::ByteCodeInstruction::And: { this->And(); break; }
      case bytecode::ByteCodeInstruction::Or: { this->Or(); break; }
      case bytecode::ByteCodeInstruction::GetType: { this->GetType(); break; }
      case bytecode::ByteCodeInstruction::CastToInt: { this->CastToInt(); break; }
      case bytecode::ByteCodeInstruction::CastToFloat: { this->CastToFloat(); break; }
      case bytecode::ByteCodeInstruction::Length: { this->Length(); break; }
      case bytecode::ByteCodeInstruction::ChatAt: { this->ChatAt(); break; }
      case bytecode::ByteCodeInstruction::StringAppend: { this->StringAppend(); break; }
      case bytecode::ByteCodeInstruction::ObjectGet: { this->ObjectGet(); break; }
      case bytecode::ByteCodeInstruction::ObjectSet: { this->ObjectSet(); break; }
      case bytecode::ByteCodeInstruction::GetEnv: { this->GetEnv(); break; }
      default: {
        this->panic("Unknown bytecode found in instructions!");
        return;
      }
    }
  }

  this->heap.EndGc();

  return;
}

void runtime::VirtualMachine::popStackFrame() {
  auto currentFrame = this->stackFrame;

  if (currentFrame == nullptr) {
    this->panic("No current stack frame found!");
    return;
  }

  auto outer = currentFrame->outer;

  if (outer == std::nullopt) {
    this->panic("No outer stack frame found!");
    return;
  }

  auto outerValue = outer.value();

  this->stackFrame = outerValue;
}

void runtime::VirtualMachine::pushStackFrame(const bytecode::Function* function) {
  auto currentFrame = this->stackFrame;

  std::optional<std::shared_ptr<StackFrame>> wrappedFrame = std::nullopt;

  if (currentFrame != nullptr) {
    wrappedFrame = currentFrame;
  }

  auto newFrame = std::make_shared<StackFrame>();
  newFrame->byteCode = &function->byteCode;

  for (std::size_t i = 0; i < function->localsCount; i++) {
    Variable undefined{};
    undefined.type = VariableType::Undefined;
    newFrame->locals.push_back(undefined);
  }

  newFrame->outer = wrappedFrame;
  newFrame->programCounter = 0;

  this->stackFrame = newFrame;
}

Variable runtime::VirtualMachine::popOpStack() {
  if (this->stackFrame == nullptr) {
    this->panic("No active stack frame found to pop op stack from!");
  }

  auto opStack = &this->stackFrame->opStack;

  if (opStack->empty()) {
    this->panic("Could not pop op stack, it is empty!");
  }

  Variable topOfStack = opStack->at(opStack->size() - 1);
  opStack->pop_back();

  return topOfStack;
}

void runtime::VirtualMachine::pushOpStack(Variable v) {
  if (this->stackFrame == nullptr) {
    this->panic("No active stack frame found to push op stack!");
  }

  this->stackFrame->opStack.push_back(v);
}

void runtime::VirtualMachine::Add() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  if (!this->protectDifferentTypes(first, second)) {
    this->advance();
    return;
  }

  if (first.type == VariableType::Integer) {
    this->pushInteger(first.integerValue + second.integerValue);

  } else if (first.type == VariableType::Float) {
    this->pushFloat(first.doubleValue + second.doubleValue);

  } else {
    this->pushUndefined();
  }

  this->advance();
}

void runtime::VirtualMachine::Subtract() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  if (!this->protectDifferentTypes(first, second)) {
    this->advance();
    return;
  }

  if (first.type == VariableType::Integer) {
    this->pushInteger(first.integerValue - second.integerValue);

  } else if (first.type == VariableType::Float) {
    this->pushFloat(first.doubleValue - second.doubleValue);

  } else {
    this->pushUndefined();
  }

  this->advance();
}

void runtime::VirtualMachine::Multiply() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  if (!this->protectDifferentTypes(first, second)) {
    this->advance();
    return;
  }

  if (first.type == VariableType::Integer) {
    this->pushInteger(first.integerValue * second.integerValue);

  } else if (first.type == VariableType::Float) {
    this->pushFloat(first.doubleValue * second.doubleValue);

  } else {
    this->pushUndefined();
  }

  this->advance();
}

void runtime::VirtualMachine::Divide() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  if (!this->protectDifferentTypes(first, second)) {
    this->advance();
    return;
  }

  if (first.type == VariableType::Integer) {
    if (second.integerValue == 0) {
      this->pushUndefined();

    } else {
      this->pushInteger(first.integerValue / second.integerValue);
    }

  } else if (first.type == VariableType::Float) {
    this->pushFloat(first.doubleValue / second.doubleValue);

  } else {
    this->pushUndefined();
  }

  this->advance();
}

void runtime::VirtualMachine::Print() {
  Variable var = this->popOpStack();
  this->out << this->variableToString(var, true);
  this->pushUndefined();
  this->advance();
}

void runtime::VirtualMachine::Read() {
  std::string read;
  std::getline(this->in, read);
  auto ret = this->heap.NewString();
  ret->assign(read);
  this->pushString(ret);
  this->advance();
}

void runtime::VirtualMachine::Jump() {
  this->stackFrame->programCounter = this->getByteCodeParameter();
}
void runtime::VirtualMachine::JumpIfFalse() {
  Variable top{this->popOpStack()};

  if (!this->booleanValueOfVariable(top)) {
    this->Jump();
  } else {
    this->advance();
  }
}

void runtime::VirtualMachine::LoadIntegerConstant() {

  std::size_t index = this->getByteCodeParameter();

  if (index >= this->file->intConstants.size()) {
    this->panic("Index out of bounds in LoadIntegerConstant");
    return;
  }

  this->pushInteger(this->file->intConstants.at(index));

  this->advance();
}

void runtime::VirtualMachine::LoadFloatConstant() {
  std::size_t index = this->getByteCodeParameter();

  if (index >= this->file->floatConstants.size()) {
    this->panic("Index out of bounds in LoadFloatConstant");
    return;
  }

  this->pushFloat(this->file->floatConstants.at(index));

  this->advance();
}

void runtime::VirtualMachine::LoadStringConstant() {
  std::size_t index = this->getByteCodeParameter();

  if (index >= this->file->stringConstants.size()) {
    this->panic("Index out of bounds in LoadStringConstant");
    return;
  }

  this->pushString(&this->file->stringConstants.at(index));

  this->advance();
}

void runtime::VirtualMachine::LoadUndefinedConstant() {
  this->pushUndefined();

  this->advance();
}

void runtime::VirtualMachine::LoadBooleanTrueConstant() {
  this->pushBoolean(true);

  this->advance();
}

void runtime::VirtualMachine::LoadBooleanFalseConstant() {
  this->pushBoolean(false);

  this->advance();
}

void runtime::VirtualMachine::LoadLocal() {
  if (this->stackFrame == nullptr) {
    this->panic("No stack frame found in LoadLocal");
    return;
  }

  auto stackFrame = this->stackFrame;

  auto locals = &stackFrame->locals;

  auto index = this->getByteCodeParameter();

  if (index >= locals->size()) {
    this->panic("Index out of bounds in LoadLocal");
    return;
  }

  Variable local = locals->at(index);

  this->pushOpStack(local);

  this->advance();
}

void runtime::VirtualMachine::SetLocal() {
  if (this->stackFrame == nullptr) {
    this->panic("No stack frame found in SetLocal");
    return;
  }

  auto stackFrame = this->stackFrame;

  auto locals = &stackFrame->locals;

  auto index = this->getByteCodeParameter();

  if (index >= locals->size()) {
    this->panic("Index out of bounds in SetLocal");
    return;
  }

  Variable top = this->popOpStack();

  locals->at(index) = top;

  this->advance();
}

void runtime::VirtualMachine::MakeFn() {
  std::size_t index = this->getByteCodeParameter();

  if (index >= this->file->functions.size()) {
    this->panic("Index out of bounds in MakeFn");
    return;
  }

  auto fnPrototype = &this->file->functions.at(index);
  runtime::Function* fn = this->heap.NewFunction();
  fn->fn = fnPrototype;
  this->pushFunction(fn);

  this->advance();
}

void runtime::VirtualMachine::Return() {
  Variable top = this->popOpStack();
  this->popStackFrame();
  this->pushOpStack(top);

  this->advance();
}

void runtime::VirtualMachine::Invoke() {
  Variable top = this->popOpStack();

  if (top.type != VariableType::Function) {
    this->pushUndefined();
    this->advance();
    return;
  }

  auto fn = top.functionValue->fn;

  std::vector<runtime::Variable> args;

  for (std::size_t i = 0; i < fn->argumentCount; i++) {
    args.push_back(this->popOpStack());
  }

  this->pushStackFrame(fn);

  for (
    std::size_t argIndex = args.size() - 1, localIndex = 0;
    localIndex < args.size();
    localIndex++, argIndex--
  ) {
    this->stackFrame->locals.at(localIndex) = args.at(argIndex);
  }
}

void runtime::VirtualMachine::MakeObj() {
  ni();
  this->advance();
}

void runtime::VirtualMachine::Less() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  if (!this->protectDifferentTypes(first, second)) {
    this->advance();
    return;
  }

  if (first.type == VariableType::Integer) {
    this->pushBoolean(first.integerValue < second.integerValue);

  } else if (first.type == VariableType::Float) {
    this->pushBoolean(first.doubleValue < second.doubleValue);

  } else {
    this->pushUndefined();
  }
  this->advance();
}

void runtime::VirtualMachine::LessOrEqual() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  if (!this->protectDifferentTypes(first, second)) {
    this->advance();
    return;
  }

  if (first.type == VariableType::Integer) {
    this->pushBoolean(first.integerValue <= second.integerValue);

  } else if (first.type == VariableType::Float) {
    this->pushBoolean(first.doubleValue <= second.doubleValue);

  } else {
    this->pushUndefined();
  }
  this->advance();
}

void runtime::VirtualMachine::Greater() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  if (!this->protectDifferentTypes(first, second)) {
    this->advance();
    return;
  }

  if (first.type == VariableType::Integer) {
    this->pushBoolean(first.integerValue > second.integerValue);

  } else if (first.type == VariableType::Float) {
    this->pushBoolean(first.doubleValue > second.doubleValue);

  } else {
    this->pushUndefined();
  }
  this->advance();
}

void runtime::VirtualMachine::GreaterOrEqual() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  if (!this->protectDifferentTypes(first, second)) {
    this->advance();
    return;
  }

  if (first.type == VariableType::Integer) {
    this->pushBoolean(first.integerValue >= second.integerValue);

  } else if (first.type == VariableType::Float) {
    this->pushBoolean(first.doubleValue >= second.doubleValue);

  } else {
    this->pushUndefined();
  }
  this->advance();
}

void runtime::VirtualMachine::Not() {
  Variable top = this->popOpStack();
  this->pushBoolean(!this->booleanValueOfVariable(top));
  this->advance();
}

void runtime::VirtualMachine::Equal() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  this->pushBoolean(this->variableEquals(first, second));
  this->advance();
}

void runtime::VirtualMachine::NotEqual() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  this->pushBoolean(!this->variableEquals(first, second));
  this->advance();
}

void runtime::VirtualMachine::And() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  this->pushBoolean(
    this->booleanValueOfVariable(first)
    &&
    this->booleanValueOfVariable(second)
  );
  this->advance();
}

void runtime::VirtualMachine::Or() {
 Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  this->pushBoolean(
    this->booleanValueOfVariable(first)
    ||
    this->booleanValueOfVariable(second)
  );
  this->advance();
}

void runtime::VirtualMachine::GetType() {
  Variable top = this->popOpStack();

  auto str = this->heap.NewString();

  switch (top.type) {
    case VariableType::Integer: {
      str->assign("integer");
      break;
    }
    case VariableType::Float: {
      str->assign("float");
      break;
    }
    case VariableType::Function: {
      str->assign("function");
      break;
    }
    case VariableType::Object: {
      str->assign("object");
      break;
    }
    case VariableType::String: {
      str->assign("string");
      break;
    }
    case VariableType::Undefined: {
      str->assign("undefined");
      break;
    }
    case VariableType::Boolean: {
      str->assign("boolean");
      break;
    }
    default: {
      this->panic("Unknown varible type at variableEquals");
    }
  }

  this->pushString(str);
  this->advance();
}

void runtime::VirtualMachine::CastToInt() {
  Variable top = this->popOpStack();

  switch (top.type) {
    case VariableType::Integer: {
      this->pushInteger(top.integerValue);
      break;
    }
    case VariableType::Float: {
      this->pushInteger(static_cast<std::int64_t>(top.doubleValue));
      break;
    }
    case VariableType::Function: {
      this->pushUndefined();
      break;
    }
    case VariableType::Object: {
      this->pushUndefined();
      break;
    }
    case VariableType::String: {
      try {
        std::int64_t val = std::stoll(*top.stringValue);
        this->pushInteger(val);

      } catch (...) {
        this->pushUndefined();
      }
      break;
    }
    case VariableType::Undefined: {
      this->pushUndefined();
      break;
    }
    case VariableType::Boolean: {
      this->pushUndefined();
      break;
    }
    default: {
      this->panic("Unknown varible type at variableEquals");
    }
  }

  this->advance();
}

void runtime::VirtualMachine::CastToFloat() {
  Variable top = this->popOpStack();

  switch (top.type) {
    case VariableType::Integer: {
      this->pushFloat(static_cast<double>(top.integerValue));
      break;
    }
    case VariableType::Float: {
      this->pushFloat(top.doubleValue);
      break;
    }
    case VariableType::Function: {
      this->pushUndefined();
      break;
    }
    case VariableType::Object: {
      this->pushUndefined();
      break;
    }
    case VariableType::String: {
      try {
        double val = std::stod(*top.stringValue);
        this->pushFloat(val);

      } catch (...) {
        this->pushUndefined();
      }
      break;
    }
    case VariableType::Undefined: {
      this->pushUndefined();
      break;
    }
    case VariableType::Boolean: {
      this->pushUndefined();
      break;
    }
    default: {
      this->panic("Unknown varible type at variableEquals");
    }
  }

  this->advance();
}

void runtime::VirtualMachine::Length() {
  Variable top = this->popOpStack();

  switch (top.type) {
    case VariableType::Integer: {
      this->pushUndefined();
      break;
    }
    case VariableType::Float: {
      this->pushUndefined();
      break;
    }
    case VariableType::Function: {
      this->pushUndefined();
      break;
    }
    case VariableType::Object: {
      this->pushInteger(top.objectValue->properties.size());
      break;
    }
    case VariableType::String: {
      this->pushInteger(top.stringValue->size());
      break;
    }
    case VariableType::Undefined: {
      this->pushUndefined();
      break;
    }
    case VariableType::Boolean: {
      this->pushUndefined();
      break;
    }
    default: {
      this->panic("Unknown varible type at variableEquals");
    }
  }

  this->advance();
}

void runtime::VirtualMachine::ChatAt() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  if (first.type != VariableType::String) {
    this->pushUndefined();
    this->advance();
    return;
  }

  if (second.type != VariableType::Integer) {
    this->pushUndefined();
    this->advance();
    return;
  }

  std::size_t index = static_cast<std::size_t>(second.integerValue);

  if (index >= first.stringValue->size()) {
    this->pushUndefined();
    this->advance();
    return;
  }

  char c = first.stringValue->at(index);

  auto str = this->heap.NewString();

  str->clear();
  *str += c;

  this->pushString(str);
  this->advance();
}

void runtime::VirtualMachine::StringAppend() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  auto str = this->heap.NewString();

  str->clear();

  str->append(this->variableToString(first, true));
  str->append(this->variableToString(second, true));

  this->pushString(str);
  this->advance();
}

void runtime::VirtualMachine::ObjectGet() {
  ni();
  this->advance();
}

void runtime::VirtualMachine::ObjectSet() {
  ni();
  this->advance();
}

void runtime::VirtualMachine::GetEnv() {
  ni();
  this->advance();
}

bool VirtualMachine::variableEquals(Variable var1, Variable var2) {
  if (var1.type != var2.type) {
    return false;
  }

  switch (var1.type) {
    case VariableType::Integer: {
      return var1.integerValue == var2.integerValue;
    }
    case VariableType::Float: {
      return var1.doubleValue == var2.doubleValue;
    }
    case VariableType::Function: {
      return var1.functionValue == var2.functionValue;
    }
    case VariableType::Object: {
      return var1.objectValue == var2.objectValue;
    }
    case VariableType::String: {
      return *var1.stringValue == *var2.stringValue;
    }
    case VariableType::Undefined: {
      return true;
    }
    case VariableType::Boolean: {
      return var1.boolValue == var2.boolValue;
    }
    default: {
      this->panic("Unknown varible type at variableEquals");
    }
  }
}

std::string VirtualMachine::variableToString(Variable var, bool panic) {
  switch (var.type) {
    case VariableType::Integer: {
      return std::to_string(var.integerValue);
    }
    case VariableType::Float: {
      return std::to_string(var.doubleValue);
    }
    case VariableType::Function: {
      return "<function>";
    }
    case VariableType::Object: {
      return "<object>";
    }
    case VariableType::String: {
      return *var.stringValue;
    }
    case VariableType::Undefined: {
      return "undefined";
    }
    case VariableType::Boolean: {
      return var.boolValue ? "true" : "false";
    }
    default: {
      if (panic) {
        this->panic("Unknown varible type at print");
      } else {
        return "UKNOWN!";
      }
    }
  }
}

bool runtime::VirtualMachine::booleanValueOfVariable(Variable var) {
  switch (var.type) {
    case VariableType::Integer:
    case VariableType::Float:
    case VariableType::Function:
    case VariableType::Object:
    case VariableType::String: {
      return true;
    }
    case VariableType::Undefined: {
      return false;
    }
    case VariableType::Boolean: {
      return var.boolValue;
    }
    default: {
      this->panic("Unknown varible type at jumpIfFalse");
    }
  }
}

void runtime::VirtualMachine::pushUndefined() {
  Variable undefined{};
  undefined.type = VariableType::Undefined;
  this->pushOpStack(undefined);
}

void runtime::VirtualMachine::pushInteger(std::int64_t val) {
  Variable variable{};
  variable.type = VariableType::Integer;
  variable.integerValue = val;
  this->pushOpStack(variable);
}

void runtime::VirtualMachine::pushFloat(double val) {
  Variable variable{};
  variable.type = VariableType::Float;
  variable.doubleValue = val;
  this->pushOpStack(variable);
}

void runtime::VirtualMachine::pushBoolean(bool val) {
  Variable variable{};
  variable.type = VariableType::Boolean;
  variable.boolValue = val;
  this->pushOpStack(variable);
}

void runtime::VirtualMachine::pushString(const std::string* val) {
  Variable variable{};
  variable.type = VariableType::String;
  variable.stringValue = val;
  this->pushOpStack(variable);
}

void runtime::VirtualMachine::pushFunction(runtime::Function* fn) {
  Variable variable{};
  variable.type = VariableType::Function;
  variable.functionValue = fn;
  this->pushOpStack(variable);
}

void runtime::VirtualMachine::pushObject(runtime::Object* obj) {
  Variable variable{};
  variable.type = VariableType::Object;
  variable.objectValue = obj;
  this->pushOpStack(variable);
}

std::size_t runtime::VirtualMachine::getByteCodeParameter() {
  return this->stackFrame->byteCode->at(this->stackFrame->programCounter).parameter;
}

bool runtime::VirtualMachine::protectDifferentTypes(Variable v1, Variable v2) {
  if (v1.type != v2.type) {
    this->pushUndefined();
    return false;
  }

  return true;
}

std::string runtime::VirtualMachine::byteCodeToString(bytecode::ByteCode bc, bool panic) {

  #define PARAM "(" + std::to_string(bc.parameter) + ")"

  switch (bc.instruction) {
    case bytecode::ByteCodeInstruction::Halt: return "Halt";
    case bytecode::ByteCodeInstruction::Add: return "Add";
    case bytecode::ByteCodeInstruction::Subtract: return "Subtract";
    case bytecode::ByteCodeInstruction::Multiply: return "Multiply";
    case bytecode::ByteCodeInstruction::Divide: return "Divide";
    case bytecode::ByteCodeInstruction::Print: return "Print";
    case bytecode::ByteCodeInstruction::Read: return "Read";
    case bytecode::ByteCodeInstruction::Jump: return "Jump" PARAM;
    case bytecode::ByteCodeInstruction::JumpIfFalse: return "JumpIfFalse" PARAM;
    case bytecode::ByteCodeInstruction::LoadIntegerConstant: return "LoadIntegerConstant" PARAM;
    case bytecode::ByteCodeInstruction::LoadFloatConstant: return "LoadFloatConstant" PARAM;
    case bytecode::ByteCodeInstruction::LoadStringConstant: return "LoadStringConstant" PARAM;
    case bytecode::ByteCodeInstruction::LoadUndefinedConstant: return "LoadUndefinedConstant";
    case bytecode::ByteCodeInstruction::LoadBooleanTrueConstant: return "LoadBooleanTrueConstant";
    case bytecode::ByteCodeInstruction::LoadBooleanFalseConstant: return "LoadBooleanFalseConstant";
    case bytecode::ByteCodeInstruction::LoadLocal: return "LoadLocal" PARAM;
    case bytecode::ByteCodeInstruction::SetLocal: return "SetLocal" PARAM;
    case bytecode::ByteCodeInstruction::Return: return "Return";
    case bytecode::ByteCodeInstruction::Invoke: return "Invoke";
    case bytecode::ByteCodeInstruction::NoOp: return "NoOp";
    case bytecode::ByteCodeInstruction::MakeFn: return "MakeFn" PARAM;
    case bytecode::ByteCodeInstruction::MakeObj: return "MakeObj" PARAM;
    case bytecode::ByteCodeInstruction::Less: return "Less";
    case bytecode::ByteCodeInstruction::LessOrEqual: return "LessOrEqual";
    case bytecode::ByteCodeInstruction::Greater: return "Greater";
    case bytecode::ByteCodeInstruction::GreaterOrEqual: return "GreaterOrEqual";
    case bytecode::ByteCodeInstruction::Not: return "Not";
    case bytecode::ByteCodeInstruction::Equal: return "Equal";
    case bytecode::ByteCodeInstruction::NotEqual: return "NotEqual";
    case bytecode::ByteCodeInstruction::And: return "And";
    case bytecode::ByteCodeInstruction::Or: return "Or";
    case bytecode::ByteCodeInstruction::GetType: return "GetType";
    case bytecode::ByteCodeInstruction::CastToInt: return "CastToInt";
    case bytecode::ByteCodeInstruction::CastToFloat: return "CastToFloat";
    case bytecode::ByteCodeInstruction::Length: return "Length";
    case bytecode::ByteCodeInstruction::ChatAt: return "ChatAt";
    case bytecode::ByteCodeInstruction::StringAppend: return "StringAppend";
    case bytecode::ByteCodeInstruction::ObjectGet: return "ObjectGet";
    case bytecode::ByteCodeInstruction::ObjectSet: return "ObjectSet";
    case bytecode::ByteCodeInstruction::GetEnv: return "GetEnv";
    default: {
      if (panic) {
        this->panic("Unkown bytecode instruction encountered");
        return "";
      } else {
        return "<UNKNOWN>";
      }
    }
  }

  #undef PARAM
}

template<typename T>
void printConstantArray(std::ostream & out, const std::vector<T> & vec) {
  std::size_t i = 0;
  for (const auto& val : vec) {
    out << "| |" << i << "| " << val << '\n';
    i++;
  }
}

void runtime::VirtualMachine::printFunction(const bytecode::Function* fn) {
  this->out << "| | Argument Count: " << fn->argumentCount << '\n';
  this->out << "| | Local Count: " << fn->localsCount << '\n';
  this->out << "| | Byte Code:\n";
  for (std::size_t i = 0; i < fn->byteCode.size(); i++) {
    this->out << "| |   |" << i << "| " << this->byteCodeToString(fn->byteCode.at(i), false) << '\n';
  }
  this->out << "| \\------------------\n";
}

void runtime::VirtualMachine::print() {
  this->out << "Stack Frames:\n";

  StackFrame* stackFrame = this->stackFrame.get();

  int frameDepth = 0;

  while (stackFrame != nullptr) {
    this->out << "| Stack Frame: " << frameDepth << '\n';
    this->out << "| | Program Counter: " << stackFrame->programCounter << '\n';
    this->out << "| | Locals:\n";
    for (std::size_t i = 0; i < stackFrame->locals.size(); i++) {
      this->out << "| |   |" << i << "| " << this->variableToString(stackFrame->locals.at(i), false) << '\n';
    }
    this->out << "| | Op Stack:\n";
    for (std::size_t i = 0; i < stackFrame->opStack.size(); i++) {
      this->out << "| |   |" << i << "| " << this->variableToString(stackFrame->opStack.at(i), false) << '\n';
    }
    this->out << "| | Byte Code:\n";
    for (std::size_t i = 0; i < stackFrame->byteCode->size(); i++) {
      this->out << "| |   |" << i << "| " << this->byteCodeToString(stackFrame->byteCode->at(i), false) << '\n';
    }
    this->out << "| \\------------------\n";

    if (stackFrame->outer) {
      stackFrame = stackFrame->outer->get();
    } else {
      stackFrame = nullptr;
    }
  }
  this->out << "\\------------------\n";

  this->out << "Entrypoint:\n";
  this->printFunction(&this->file->entrypoint);
  this->out << "\\------------------\n";

  this->out << "Functions:\n";
  for (std::size_t i = 0; i < this->file->functions.size(); i++) {
    this->out << "| Function: " << i << '\n';
    this->printFunction(&this->file->functions.at(i));
  }
  this->out << "\\------------------\n";

  this->out << "Object Constructors:\n";
  for (std::size_t i = 0; i < this->file->objects.size(); i++) {
    this->out << "| |" << i << "| ";
    const auto& obj = this->file->objects.at(i);
    for (const auto& key : obj.keys) {
      this->out << key << ", ";
    }
    this->out << '\n';
  }
  this->out << "\\------------------\n";

  this->out << "Integer Constants:\n";
  printConstantArray<std::int64_t>(this->out, this->file->intConstants);
  this->out << "\\------------------\n";

  this->out << "Float Constants:\n";
  printConstantArray<double>(this->out, this->file->floatConstants);
  this->out << "\\------------------\n";

  this->out << "String Constants:\n";
  printConstantArray<std::string>(this->out, this->file->stringConstants);
  this->out << "\\------------------\n";

  this->out << std::endl;
}

void runtime::VirtualMachine::panic(const std::string & message) {
  if (this->isPanicing) {
    this->out << "RECURSIVE CALL TO PANIC, JUST EXITING!" << std::endl;
    exit(1);
  }

  this->isPanicing = true;

  this->out << "PANIC!: " << message << "\n";

  this->print();

  exit(1);
}

void runtime::VirtualMachine::advance() {
  this->stackFrame->programCounter++;
}

runtime::Heap::Heap(runtime::VirtualMachine* /*vm*/) noexcept {

}

runtime::Heap::~Heap() noexcept {

}

void runtime::Heap::StartGc() noexcept {

}

template<typename T>
void deInitList(std::list<T*> & items) {
  while (!items.empty()) {
    T* first = items.front();
    delete first;
    items.pop_front();
  }
}

void runtime::Heap::EndGc() noexcept {
  deInitList<std::string>(this->createdStrings);
  deInitList<runtime::Function>(this->createdFunctions);
  deInitList<runtime::Object>(this->createdObjects);
}

std::string* runtime::Heap::NewString() noexcept {
  auto ret = new std::string{""};
  this->createdStrings.push_back(ret);
  return ret;
}

runtime::Function* runtime::Heap::NewFunction() noexcept {
  auto ret = new runtime::Function{};
  this->createdFunctions.push_back(ret);
  return ret;
}

runtime::Object* runtime::Heap::NewObject() noexcept {
  auto ret = new runtime::Object{};
  this->createdObjects.push_back(ret);
  return ret;
}

}