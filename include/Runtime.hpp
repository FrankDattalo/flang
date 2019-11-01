#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include "lib.hpp"
#include "ByteCode.hpp"

namespace runtime {

struct StackFrame;

struct Variable;

struct Function;

class VirtualMachine {
private:
  const std::shared_ptr<const bytecode::CompiledFile> file;
  std::shared_ptr<StackFrame> stackFrame = nullptr;

  std::ostream & out;
  std::istream & in;
  bool isPanicing;

public:
  explicit VirtualMachine(
    std::ostream & out,
    std::istream & in,
    std::shared_ptr<const bytecode::CompiledFile> file
  ) noexcept
  : file{std::move(file)}
  , out{out}
  , in{in}
  , isPanicing{false}
  {}

  virtual ~VirtualMachine() = default;

  void run() noexcept;

private:
  void pushStackFrame(const bytecode::Function* function);

  void popStackFrame();

  void panic(const std::string & message);

  Variable popOpStack();

  void pushOpStack(Variable v);

  void Add();

  void Subtract();

  void Multiply();

  void Divide();

  void Pop();

  void Print();

  void Read();

  void Jump();

  void JumpIfFalse();

  void LoadConstant();

  void LoadLocal();

  void SetLocal();

  void Return();

  void Invoke();

  void MakeFn();

  void Less();

  void LessOrEqual();

  void Greater();

  void GreaterOrEqual();

  bool protectDifferentTypes(Variable v1, Variable v2);

  void pushUndefined();

  void pushInteger(std::int64_t val);

  void pushFloat(double val);

  void pushBoolean(bool val);

  void pushString(const std::string* str);

  std::size_t getByteCodeParameter();

  std::string variableToString(Variable var, bool panic);

  std::string byteCodeToString(bytecode::ByteCode bc, bool panic);

  void printFunction(const bytecode::Function* fn);
};

}

#endif
