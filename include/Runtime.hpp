#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include "lib.hpp"

enum class ObjectType {
  Undefined,
  Integer,
  Boolean,
  Float,
  String,
  Object,
  Function,
};

struct Variable;
struct Object;
struct Function;
struct ByteCode;

struct CompiledFile {
  ByteCode* byteCode;
  std::vector<Function> functions;
  std::vector<Variable> constants;
};

struct ByteCode {
  std::vector<std::int64_t> code;
};

struct Closure {
  const Function* function;
};

struct Function {
  const std::size_t argCount;
  ByteCode* byteCode;
};

struct Variable {
  ObjectType type;

  union {
    std::int64_t integerValue;
    double doubleValue;
    bool boolValue;
    Object* objectValue;
    Closure* closureValue;
    std::string* stringValue;
  };

  static Variable createUndefined();

  static Variable createBoolean(bool val);

  static Variable createInteger(std::int64_t val);

  static Variable createObject(Object* val);

  static Variable createFloat(double val);

  static Variable createString(const std::string * str);

  bool equals(const Variable& other);
};

struct Object {
  std::unordered_map<std::string, Variable> properties;
};

namespace Runtime {
  void panic(const std::string & errorMessage);
}

#endif
