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

struct Variable;

struct Object {
  std::unordered_map<std::string, Variable> properties;
};

struct Function {
  bytecode::Function* fn;
};

struct String {
  bool isConstant;
  std::string* str;
};

struct Variable {
  VariableType type;

  union {
    std::int64_t integerValue;
    double doubleValue;
    bool boolValue;
    Object* objectValue;
    Function* functionValue;
    String* stringValue;
  };
};

struct StackFrame {
  std::size_t programCounter;
  std::vector<Variable> locals;
  const std::vector<const bytecode::ByteCode>* byteCode;
  std::optional<std::shared_ptr<StackFrame>> outer;
  std::vector<Variable> opStack;
};

#define handleOpCode(type) case bytecode::ByteCodeInstruction::##type: { this->##type(); break; }

void VirtualMachine::run() noexcept {

  this->pushStackFrame(this->file->entrypoint);

  while (true) {
    auto pg = this->stackFrame->programCounter;

    if (pg >= this->stackFrame->byteCode->size()) {
      this->panic("Program counter overran bytecode!");
      return;
    }

    auto instruction = this->stackFrame->byteCode->at(pg).instruction;

    switch (instruction) {
      case bytecode::ByteCodeInstruction::Halt: {
        return;
      }
      handleOpCode(Add)
      handleOpCode(Subtract)
      handleOpCode(Multiply)
      handleOpCode(Divide)
      handleOpCode(Pop)
      handleOpCode(Print)
      handleOpCode(Read)
      handleOpCode(Jump)
      handleOpCode(JumpIfFalse)
      handleOpCode(LoadConstant)
      handleOpCode(LoadLocal)
      handleOpCode(SetLocal)
      handleOpCode(Return)
      handleOpCode(Invoke)
      default: {
        this->panic("Unknown bytecode found in instructions!");
        return;
      }
    }
  }

  return;
}

#undef handleOpCode

void VirtualMachine::popStackFrame() {
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

void VirtualMachine::pushStackFrame(const bytecode::Function& function) {
  auto currentFrame = this->stackFrame;

  std::optional<std::shared_ptr<StackFrame>> wrappedFrame = std::nullopt;

  if (currentFrame != nullptr) {
    wrappedFrame = currentFrame;
  }

  auto newFrame = std::make_shared<StackFrame>();
  newFrame->byteCode = &function.byteCode;

  for (std::size_t i = 0; i < function.localsCount; i++) {
    Variable undefined;
    undefined.type = VariableType::Undefined;
    newFrame->locals.push_back(undefined);
  }

  newFrame->outer = wrappedFrame;
  newFrame->programCounter = 0;

  this->stackFrame = newFrame;
}

Variable VirtualMachine::popOpStack() {
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

void VirtualMachine::pushOpStack(Variable v) {
  if (this->stackFrame == nullptr) {
    this->panic("No active stack frame found to push op stack!");
  }

  auto opStack = &this->stackFrame->opStack;

  opStack->push_back(v);
}

void VirtualMachine::Add() {
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

void VirtualMachine::Subtract() {
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

void VirtualMachine::Multiply() {
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

void VirtualMachine::Divide() {
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

void VirtualMachine::Pop() {
  this->popOpStack();
}

void VirtualMachine::Print() {

}

void VirtualMachine::Read() {

}

void VirtualMachine::Jump() {
  this->stackFrame->programCounter = this->getByteCodeParameter();
}

void VirtualMachine::JumpIfFalse() {
  Variable top = this->popOpStack();

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

void VirtualMachine::LoadConstant() {

}

void VirtualMachine::LoadLocal() {

}

void VirtualMachine::SetLocal() {

}

void VirtualMachine::Return() {
  this->popStackFrame();
}

void VirtualMachine::Invoke() {

}

void VirtualMachine::pushUndefined() {
  Variable undefined;
  undefined.type = VariableType::Undefined;
  this->pushOpStack(undefined);
}

bool VirtualMachine::protectDifferentTypes(Variable v1, Variable v2) {
  if (v1.type != v2.type) {
    this->pushUndefined();
    return false;
  }

  return true;
}

void VirtualMachine::panic(const std::string & message) {
  this->out << message << std::endl;
  exit(1);
}

}