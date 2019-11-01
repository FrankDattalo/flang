#include "Runtime.hpp"

namespace runtime {

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
  bytecode::Function* fn;
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

  while (true) {
    if (this->stackFrame->programCounter >= this->stackFrame->byteCode->size()) {
      this->panic("Program counter overran bytecode!");
      return;
    }

    auto instruction = this->stackFrame->byteCode->at(this->stackFrame->programCounter).instruction;

    switch (instruction) {
      case bytecode::ByteCodeInstruction::Halt: { return; }
      case bytecode::ByteCodeInstruction::Add: { this->Add(); break; }
      case bytecode::ByteCodeInstruction::Subtract: { this->Subtract(); break; }
      case bytecode::ByteCodeInstruction::Multiply: { this->Multiply(); break; }
      case bytecode::ByteCodeInstruction::Divide: { this->Divide(); break; }
      case bytecode::ByteCodeInstruction::Pop: { this->Pop(); break; }
      case bytecode::ByteCodeInstruction::Print: { this->Print(); break; }
      case bytecode::ByteCodeInstruction::Read: { this->Read(); break; }
      case bytecode::ByteCodeInstruction::Jump: { this->Jump(); break; }
      case bytecode::ByteCodeInstruction::JumpIfFalse: { this->JumpIfFalse(); break; }
      case bytecode::ByteCodeInstruction::LoadLocal: { this->LoadLocal(); break; }
      case bytecode::ByteCodeInstruction::SetLocal: { this->SetLocal(); break; }
      case bytecode::ByteCodeInstruction::LoadConstant: { this->LoadConstant(); break; }
      case bytecode::ByteCodeInstruction::Return: { this->Return(); break; }
      case bytecode::ByteCodeInstruction::Invoke: { this->Invoke(); break; }
      case bytecode::ByteCodeInstruction::MakeFn: { this->MakeFn(); break; }
      case bytecode::ByteCodeInstruction::NoOp: { break; }
      default: {
        this->panic("Unknown bytecode found in instructions!");
        return;
      }
    }

    this->stackFrame->programCounter++;
  }

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
    return;
  }

  if (first.type == VariableType::Integer) {
    this->pushInteger(first.integerValue + second.integerValue);

  } else if (first.type == VariableType::Float) {
    this->pushFloat(first.doubleValue + second.doubleValue);

  } else {
    this->pushUndefined();
  }
}

void runtime::VirtualMachine::Subtract() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  if (!this->protectDifferentTypes(first, second)) {
    return;
  }

  if (first.type == VariableType::Integer) {
    this->pushInteger(first.integerValue - second.integerValue);

  } else if (first.type == VariableType::Float) {
    this->pushFloat(first.doubleValue - second.doubleValue);

  } else {
    this->pushUndefined();
  }
}

void runtime::VirtualMachine::Multiply() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  if (!this->protectDifferentTypes(first, second)) {
    return;
  }

  if (first.type == VariableType::Integer) {
    this->pushInteger(first.integerValue * second.integerValue);

  } else if (first.type == VariableType::Float) {
    this->pushFloat(first.doubleValue * second.doubleValue);

  } else {
    this->pushUndefined();
  }
}

void runtime::VirtualMachine::Divide() {
  Variable second = this->popOpStack();
  Variable first = this->popOpStack();

  if (!this->protectDifferentTypes(first, second)) {
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
}

void runtime::VirtualMachine::Pop() {
  this->popOpStack();
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
      return std::to_string(var.boolValue);
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

void runtime::VirtualMachine::Print() {
  Variable var = this->popOpStack();
  this->out << this->variableToString(var, true);
}

void runtime::VirtualMachine::Read() {
  std::string read;
  std::getline(this->in, read);
}

void runtime::VirtualMachine::Jump() {
  this->stackFrame->programCounter = this->getByteCodeParameter();
}

void runtime::VirtualMachine::JumpIfFalse() {
  Variable top{this->popOpStack()};

  switch (top.type) {
    case VariableType::Integer:
    case VariableType::Float:
    case VariableType::Function:
    case VariableType::Object:
    case VariableType::String: {
      return;
    }
    case VariableType::Undefined: {
      this->Jump();
      return;
    }
    case VariableType::Boolean: {
      if (!top.boolValue) {
        this->Jump();
      }
      return;
    }
    default: {
      this->panic("Unknown varible type at jumpIfFalse");
    }
  }
}

void runtime::VirtualMachine::LoadConstant() {
  auto index = this->getByteCodeParameter();

  if (index >= this->file->constants.size()) {
    this->panic("Constant index out of bounds!");
  }

  bytecode::Constant constant = this->file->constants.at(index);

  switch (constant.type) {
    case bytecode::ConstantType::Boolean: {
      this->pushBoolean(constant.boolValue);
      return;
    }
    case bytecode::ConstantType::Integer: {
      this->pushInteger(constant.intValue);
      return;
    }
    case bytecode::ConstantType::Float: {
      this->pushFloat(constant.floatValue);
      return;
    }
    case bytecode::ConstantType::Undefined: {
      this->pushUndefined();
      return;
    }
    case bytecode::ConstantType::String: {
      this->pushString(constant.stringValue);
      return;
    }
  }
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
}

void runtime::VirtualMachine::SetLocal() {
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

  Variable top = this->popOpStack();

  locals->at(index) = top;
}

void runtime::VirtualMachine::MakeFn() {

}

void runtime::VirtualMachine::Return() {
  Variable top = this->popOpStack();
  this->popStackFrame();
  this->pushOpStack(top);
}

void runtime::VirtualMachine::Invoke() {
  auto index = this->getByteCodeParameter();

  if (index >= this->file->functions.size()) {
    this->panic("Function index out of range in Invoke");
    return;
  }

  auto fn = &this->file->functions.at(index);

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
  variable.doubleValue = val;
  this->pushOpStack(variable);
}

void runtime::VirtualMachine::pushString(const std::string* val) {
  Variable variable{};
  variable.type = VariableType::String;
  variable.stringValue = val;
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
  switch (bc.instruction) {
    case bytecode::ByteCodeInstruction::Halt: return "Halt";
    case bytecode::ByteCodeInstruction::Add: return "Add";
    case bytecode::ByteCodeInstruction::Subtract: return "Subtract";
    case bytecode::ByteCodeInstruction::Multiply: return "Multiply";
    case bytecode::ByteCodeInstruction::Divide: return "Divide";
    case bytecode::ByteCodeInstruction::Pop: return "Pop";
    case bytecode::ByteCodeInstruction::Print: return "Print";
    case bytecode::ByteCodeInstruction::Read: return "Read";
    case bytecode::ByteCodeInstruction::Jump: return "Jump";
    case bytecode::ByteCodeInstruction::JumpIfFalse: return "JumpIfFalse";
    case bytecode::ByteCodeInstruction::LoadConstant: return "LoadConstant(" + std::to_string(bc.parameter) + ")";
    case bytecode::ByteCodeInstruction::LoadLocal: return "LoadLocal(" + std::to_string(bc.parameter) + ")";
    case bytecode::ByteCodeInstruction::SetLocal: return "SetLocal(" + std::to_string(bc.parameter) + ")";
    case bytecode::ByteCodeInstruction::Return: return "Return";
    case bytecode::ByteCodeInstruction::Invoke: return "Invoke(" + std::to_string(bc.parameter) + ")";
    case bytecode::ByteCodeInstruction::NoOp: return "NoOp";
    default: {
      if (panic) {
        this->panic("Unkown bytecode instruction encountered");
        return "";
      } else {
        return "<UNKNOWN>";
      }
    }
  }
}

void runtime::VirtualMachine::printFunction(const bytecode::Function* fn) {
  this->out << "| | Argument Count: " << fn->argumentCount << '\n';
  this->out << "| | Local Count: " << fn->localsCount << '\n';
  this->out << "| | Byte Code:\n";
  for (std::size_t i = 0; i < stackFrame->byteCode->size(); i++) {
    this->out << "| |   |" << i << "| " << this->byteCodeToString(fn->byteCode.at(i), false) << '\n';
  }
  this->out << "| \\------------------\n";
}

void runtime::VirtualMachine::panic(const std::string & message) {
  if (this->isPanicing) {
    this->out << "RECURSIVE CALL TO PANIC, JUST EXITING!" << std::endl;
    exit(1);
  }

  this->isPanicing = true;

  this->out << "PANIC!: " << message << "\n";

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
    this->out << "  Function: " << i << '\n';
    this->printFunction(&this->file->functions.at(i));
  }
  this->out << "\\------------------\n";

  // TODO: print constants

  // TODO: print object constructors

  this->out << std::endl;

  exit(1);
}

}