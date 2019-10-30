#include "ByteCode.hpp"

inline ByteCodeInstruction intToByteCodeInstruction(std::int64_t val) noexcept {

  if (val < 0) {
    return ByteCodeInstruction::Halt;
  }

  ByteCodeInstruction casted = static_cast<ByteCodeInstruction>(val);

  if (casted >= ByteCodeInstruction::_Last) {
    return ByteCodeInstruction::Halt;
  }

  return casted;
}