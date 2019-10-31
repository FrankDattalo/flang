#include "lib.hpp"
#include "Runtime.hpp"

int main(int  /*argc*/, char**  /*argv*/) {

  auto compiledCode = std::make_shared<bytecode::CompiledFile>(
    bytecode::Function{0, 5, {
        bytecode::ByteCode(bytecode::ByteCodeInstruction::LoadConstant, 0),
        bytecode::ByteCode(bytecode::ByteCodeInstruction::LoadConstant, 1),
        bytecode::ByteCode(bytecode::ByteCodeInstruction::LoadConstant, 2),
        bytecode::ByteCode(bytecode::ByteCodeInstruction::Add, 0),
        bytecode::ByteCode(bytecode::ByteCodeInstruction::Print, 0),
        bytecode::ByteCode(bytecode::ByteCodeInstruction::Halt, 0),
      }
    },
    std::vector<bytecode::Function>{},
    std::vector<bytecode::ObjectConstructor>{},
    std::vector<bytecode::Constant>{
      bytecode::Constant(static_cast<std::int64_t>(1)),
      bytecode::Constant(static_cast<std::int64_t>(2)),
    },
    std::vector<std::string>{}
  );

  auto vm = std::make_shared<runtime::VirtualMachine>(std::cout, std::cin, compiledCode);

  vm->run();

  return 0;
}