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
  Print, // 1 arg, returns undefined
  Read, // no args, returns string
  Jump,
  JumpIfFalse,
  LoadIntegerConstant,
  LoadFloatConstant,
  LoadStringConstant,
  LoadUndefinedConstant,
  LoadBooleanTrueConstant,
  LoadBooleanFalseConstant,
  LoadLocal,
  SetLocal,
  Return,
  Invoke,
  NoOp,
  MakeFn,
  MakeObj,
  Less,
  LessOrEqual,
  Greater,
  GreaterOrEqual,
  Not,
  Equal,
  NotEqual,
  And,
  Or,
  GetType,
  CastToInt,
  CastToFloat,
  Length,
  ChatAt,
  StringAppend,
  ObjectGet,
  ObjectSet,
  GetEnv, // no args, returns object
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

struct CompiledFile {

  const Function entrypoint;
  const std::vector<Function> functions;
  const std::vector<ObjectConstructor> objects;
  const std::vector<std::int64_t> intConstants;
  const std::vector<double> floatConstants;
  const std::vector<std::string> stringConstants;

  explicit CompiledFile(
    const Function entrypoint,
    const std::vector<Function> functions,
    const std::vector<ObjectConstructor> objects,
    const std::vector<std::int64_t> intConstants,
    const std::vector<double> floatConstants,
    const std::vector<std::string> stringConstants
  ) noexcept
  : entrypoint{std::move(entrypoint)}
  , functions{std::move(functions)}
  , objects{std::move(objects)}
  , intConstants{std::move(intConstants)}
  , floatConstants{std::move(floatConstants)}
  , stringConstants{std::move(stringConstants)}
  {}

};

}

#endif
