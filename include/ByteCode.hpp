#ifndef BYTECODE_HPP
#define BYTECODE_HPP

#include "lib.hpp"

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
  _Last,
};

inline ByteCodeInstruction intToByteCodeInstruction(std::int64_t val) noexcept;

#endif
