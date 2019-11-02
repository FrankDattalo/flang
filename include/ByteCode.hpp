#ifndef BYTECODE_HPP
#define BYTECODE_HPP

#include "lib.hpp"

namespace bytecode {

enum class ByteCodeInstruction {
  Halt,
  Add, // 2 args
  Subtract, // 2 args
  Multiply, // 2 args
  Divide, // 2 args
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
  Less, //2 args, returns bool
  LessOrEqual, //2 args, returns bool
  Greater, //2 args, returns bool
  GreaterOrEqual, //2 args, returns bool
  Not, // 1 arg, returns bool
  Equal, // 2 args, returns bool
  NotEqual, // 2 args, returns bool
  And, // 2 args, returns bool
  Or, // 2 args, returns bool
  GetType, // 1 arg, returns string
  CastToInt, // 1 arg
  CastToFloat, // 1 arg
  Length, // 1 arg
  ChatAt, // 2 args
  StringAppend, // 2 args
  ObjectGet, // 2 args
  ObjectSet, // 3 args
  GetEnv, // 1 arg, returns string
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
  const std::size_t capturesCount;
  const std::vector<ByteCode> byteCode;

  explicit Function(
    std::size_t argumentCount,
    std::size_t localsCount,
    std::size_t capturesCount,
    const std::vector<ByteCode> byteCode
  ) noexcept
  : argumentCount{argumentCount}
  , localsCount{localsCount}
  , capturesCount{capturesCount}
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
