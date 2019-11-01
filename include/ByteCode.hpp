#ifndef BYTECODE_HPP
#define BYTECODE_HPP

#include "lib.hpp"

namespace bytecode {

enum class ByteCodeInstruction {
  Halt,
  Add,
  Subtract,
  Multiply,
  Divide,
  Pop,
  Print,
  Read,
  Jump,
  JumpIfFalse,
  LoadConstant,
  LoadLocal,
  SetLocal,
  Return,
  Invoke,
  NoOp,
  MakeFn,
  Less,
  LessOrEqual,
  Greater,
  GreaterOrEqual,
  Not,
  Equal,
};

struct ByteCode {
  const ByteCodeInstruction instruction;
  const std::size_t parameter;

  explicit ByteCode(
    ByteCodeInstruction instruction,
    std::size_t parameter
  ) noexcept
  : instruction{instruction}
  , parameter{parameter}
  {}
};

struct Function {
  const std::size_t argumentCount;
  const std::size_t localsCount;
  const std::vector<ByteCode> byteCode;

  explicit Function(
    std::size_t argumentCount,
    std::size_t localsCount,
    const std::vector<ByteCode> byteCode
  ) noexcept
  : argumentCount{argumentCount}
  , localsCount{localsCount}
  , byteCode{std::move(byteCode)}
  {}
};

struct ObjectConstructor {
  const std::vector<std::string> keys;

  explicit ObjectConstructor(
    const std::vector<std::string> keys
  ) noexcept
  : keys{std::move(keys)}
  {}
};

enum class ConstantType {
  Undefined, Boolean, Integer, Float, String,
};

struct Constant {
  ConstantType type;

  union {
    bool boolValue;
    std::int64_t intValue;
    double floatValue;
    const std::string* stringValue;
  };

  explicit Constant() noexcept
  : type{ConstantType::Undefined}
  {}

  explicit Constant(bool boolValue) noexcept
  : type{ConstantType::Boolean}
  , boolValue{boolValue}
  {}

  explicit Constant(std::int64_t intValue) noexcept
  : type{ConstantType::Integer}
  , intValue{intValue}
  {}

  explicit Constant(double floatValue) noexcept
  : type{ConstantType::Float}
  , floatValue{floatValue}
  {}

  explicit Constant(const std::string& stringValue) noexcept
  : type{ConstantType::String}
  , stringValue{&stringValue}
  {}
};

struct CompiledFile {

  const Function entrypoint;
  const std::vector<Function> functions;
  const std::vector<ObjectConstructor> objects;
  const std::vector<Constant> constants;
  const std::vector<std::string> stringConstants;

  explicit CompiledFile(
    const Function entrypoint,
    const std::vector<Function> functions,
    const std::vector<ObjectConstructor> objects,
    const std::vector<Constant> constants,
    const std::vector<std::string> stringConstants
  ) noexcept
  : entrypoint{std::move(entrypoint)}
  , functions{std::move(functions)}
  , objects{std::move(objects)}
  , constants{std::move(constants)}
  , stringConstants{std::move(stringConstants)}
  {}

};

}

#endif
