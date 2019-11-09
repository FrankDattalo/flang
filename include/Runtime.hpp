#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include "lib.hpp"
#include "ByteCode.hpp"

namespace runtime {

struct StackFrame;

struct Variable;

struct Function;

struct Object;

class VirtualMachine;

class Heap {
private:
  std::list<std::string*> createdStrings;
  std::list<runtime::Function*> createdFunctions;
  std::list<runtime::Object*> createdObjects;

public:

  explicit Heap(runtime::VirtualMachine*) noexcept;

  virtual ~Heap() noexcept;

  void StartGc() noexcept;

  void EndGc() noexcept;

  std::string* NewString() noexcept;

  runtime::Function* NewFunction() noexcept;

  runtime::Object* NewObject() noexcept;

};

class VirtualMachine {
private:
  const std::shared_ptr<const bytecode::CompiledFile> file;
  std::shared_ptr<StackFrame> stackFrame;

  Heap heap;

  std::ostream & out;
  std::istream & in;
  bool isPanicing;
  bool isDebug;

public:
  explicit VirtualMachine(
    bool isDebug,
    std::ostream & out,
    std::istream & in,
    std::shared_ptr<const bytecode::CompiledFile> file
  ) noexcept
  : file{std::move(file)}
  , stackFrame{nullptr}
  , heap{this}
  , out{out}
  , in{in}
  , isPanicing{false}
  , isDebug{isDebug}
  {}

  virtual ~VirtualMachine() = default;

  void run() noexcept;

private:
  void pushStackFrame(const runtime::Function* function);

  void popStackFrame();

  [[noreturn]]
  void panic(const std::string & message);

  Variable popOpStack();

  void pushOpStack(Variable v);

  void Add();

  void Subtract();

  void Multiply();

  void Divide();

  void Print();

  void Read();

  void Jump();

  void JumpIfFalse();

  void LoadIntegerConstant();

  void LoadFloatConstant();

  void LoadStringConstant();

  void LoadUndefinedConstant();

  void LoadBooleanTrueConstant();

  void LoadBooleanFalseConstant();

  void LoadLocal();

  void LoadClosure();

  void SetLocal();

  void Return();

  void Invoke();

  void MakeFn();

  void MakeObj();

  void Less();

  void LessOrEqual();

  void Greater();

  void GreaterOrEqual();

  void Not();

  void Equal();

  void NotEqual();

  void And();

  void Or();

  void GetType();

  void CastToInt();

  void CastToFloat();

  void Length();

  void ChatAt();

  void StringAppend();

  void ObjectGet();

  void ObjectSet();

  void GetEnv();

  bool protectDifferentTypes(Variable v1, Variable v2);

  void pushUndefined();

  void pushInteger(std::int64_t val);

  void pushFloat(double val);

  void pushBoolean(bool val);

  void pushString(const std::string* str);

  void pushFunction(runtime::Function* fn);

  void pushObject(runtime::Object* obj);

  std::size_t getByteCodeParameter();

  std::string variableToString(Variable var, bool panic);

  std::string byteCodeToString(bytecode::ByteCode bc, bool panic);

  void printFunction(const bytecode::Function* fn);

  bool booleanValueOfVariable(Variable var);

  bool variableEquals(Variable var1, Variable var2);

  void print();

  void advance();

  runtime::Variable loadClosure(const bytecode::ClosureContext& closure);
};

}

#endif
