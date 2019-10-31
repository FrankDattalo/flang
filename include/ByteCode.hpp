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
};

struct ByteCode {
  const ByteCodeInstruction instruction;
  const std::size_t parameter;

  explicit ByteCode(ByteCodeInstruction instruction, std::size_t parameter) noexcept
  : instruction{instruction}
  , parameter{parameter}
  {}
};

struct Function {
  std::size_t argumentCount;
  std::size_t localsCount;
  const std::vector<const ByteCode> byteCode;

  explicit Function(
    std::size_t argumentCount,
    std::size_t localsCount,
    const std::vector<const ByteCode> byteCode
  ) noexcept
  : argumentCount{argumentCount}
  , localsCount{localsCount}
  , byteCode{std::move(byteCode)}
  {}
};

struct ObjectConstructor {
  const std::vector<const std::string> keys;

  explicit ObjectConstructor(const std::vector<const std::string> keys) noexcept
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

  const std::size_t localsCount;
  const std::vector<const ByteCode> byteCode;
  const std::vector<const Function> functions;
  const std::vector<const ObjectConstructor> objects;
  const std::vector<const Constant> constants;
  const std::vector<const std::string> stringConstants;

  explicit CompiledFile(
    const std::size_t localsCount,
    const std::vector<const ByteCode> byteCode,
    const std::vector<const Function> functions,
    const std::vector<const ObjectConstructor> objects,
    const std::vector<const Constant> constants,
    const std::vector<const std::string> stringConstants
  ) noexcept
  : localsCount{localsCount}
  , byteCode{std::move(byteCode)}
  , functions{std::move(functions)}
  , objects{std::move(objects)}
  , constants{std::move(constants)}
  , stringConstants{std::move(stringConstants)}
  {}

};

}

#endif
