#include "lib.hpp"
#include "Runtime.hpp"

/*

var x = 1;

var greet = function() {
  var _ = print("Hello, World!");
};

while (less(x, 10)) {
  var _ = print(x);
  _ = greet();
  x = add(x, 1);
}

*/


int main(int  /*argc*/, char**  /*argv*/) {

  std::vector<std::string> stringConsts = { "Hello, World\n" };

  auto compiledCode = std::make_shared<bytecode::CompiledFile>(
    bytecode::Function{
        0, 3, {
        /* var x = 1 */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadConstant, 2},
        bytecode::ByteCode{bytecode::ByteCodeInstruction::SetLocal, 0},

        /* var greet = function() { ... }; */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::MakeFn, 0},
        bytecode::ByteCode{bytecode::ByteCodeInstruction::SetLocal, 1},

        /* less(x, 10) */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadLocal, 0},
        bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadConstant, 3},
        bytecode::ByteCode{bytecode::ByteCodeInstruction::Less, 0},

        /* while */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::JumpIfFalse, 14},

        /* var _ = print(x); */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadLocal, 0},
        bytecode::ByteCode{bytecode::ByteCodeInstruction::Print, 0},
        bytecode::ByteCode{bytecode::ByteCodeInstruction::SetLocal, 2},

        /* _ = greet(); */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadLocal, 1},
        bytecode::ByteCode{bytecode::ByteCodeInstruction::Invoke, 0},
        bytecode::ByteCode{bytecode::ByteCodeInstruction::Jump, 4},

        /* end of program */
        bytecode::ByteCode(bytecode::ByteCodeInstruction::Halt, 0),
      }
    },
    std::vector<bytecode::Function>{
      bytecode::Function{
        0, 1, {
          /* var _ = print("Hello, World!"); */
          bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadConstant, 1},
          bytecode::ByteCode{bytecode::ByteCodeInstruction::Print, 0},
          bytecode::ByteCode{bytecode::ByteCodeInstruction::SetLocal, 0},

          /* implicit return */
          bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadConstant, 0},
          bytecode::ByteCode{bytecode::ByteCodeInstruction::Return, 0},
        }
      }
    },

    std::vector<bytecode::ObjectConstructor>{},

    std::vector<bytecode::Constant>{
      bytecode::Constant(),
      bytecode::Constant(stringConsts.at(0)),
      bytecode::Constant(static_cast<std::int64_t>(1)),
      bytecode::Constant(static_cast<std::int64_t>(10)),
    },
    stringConsts
  );

  auto vm = std::make_shared<runtime::VirtualMachine>(std::cout, std::cin, compiledCode);

  vm->run();

  return 0;
}