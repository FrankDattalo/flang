#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include "lib.hpp"
#include "Error.hpp"
#include "ByteCode.hpp"

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
  const std::unordered_map<const std::string, Variable> properties;
};

struct Function {
  const bytecode::Function* fn;
};

struct String {
  const bool isConstant;
  const std::string* str;
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
  const std::vector<Variable> locals;
  const bytecode::ByteCode* byteCode;
  const std::optional<std::shared_ptr<StackFrame>> outer;
};

}

#endif
