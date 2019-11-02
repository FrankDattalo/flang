#include "lib.hpp"
#include "Runtime.hpp"

/*

var x = 1;

var greet = function() {
  var _ = print("Hello, World!\n");
};

while (less(x, 10)) {
  var _ = print(x);
  _ = print("\n");
  _ = greet();
  x = add(x, 1);
}

*/


int main(int  /*argc*/, char**  /*argv*/) {

    auto compiledCode = std::make_shared<bytecode::CompiledFile>(
    bytecode::Function{
        0, 3, 0, {
        /* var x = 1 */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadIntegerConstant, 0}, //0
        bytecode::ByteCode{bytecode::ByteCodeInstruction::SetLocal, 0}, //1

        /* var greet = function() { ... }; */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::MakeFn, 0}, //2
        bytecode::ByteCode{bytecode::ByteCodeInstruction::SetLocal, 1}, //3

        /* less(x, 10) */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadLocal, 0}, //4
        bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadIntegerConstant, 1}, //5
        bytecode::ByteCode{bytecode::ByteCodeInstruction::Less, 0}, //6

        /* while */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::JumpIfFalse, 22}, //7

        /* var _ = print(x); */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadLocal, 0}, //8
        bytecode::ByteCode{bytecode::ByteCodeInstruction::Print, 0}, //9
        bytecode::ByteCode{bytecode::ByteCodeInstruction::SetLocal, 2}, //10

        /* _ = print("\n"); */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadStringConstant, 1}, //11
        bytecode::ByteCode{bytecode::ByteCodeInstruction::Print, 0}, //12
        bytecode::ByteCode{bytecode::ByteCodeInstruction::SetLocal, 2}, //13

        /* _ = greet(); */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadLocal, 1}, //14
        bytecode::ByteCode{bytecode::ByteCodeInstruction::Invoke, 0}, //15
        bytecode::ByteCode{bytecode::ByteCodeInstruction::SetLocal, 2}, //16

        /* x = add(x, 1); */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadLocal, 0}, //17
        bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadIntegerConstant, 0}, //18
        bytecode::ByteCode{bytecode::ByteCodeInstruction::Add, 0}, //19
        bytecode::ByteCode{bytecode::ByteCodeInstruction::SetLocal, 0}, //20

        /* endwhile */
        bytecode::ByteCode{bytecode::ByteCodeInstruction::Jump, 4}, //21

        /* end of program */
        bytecode::ByteCode(bytecode::ByteCodeInstruction::Halt, 0), //22
      }
    },
    std::vector<bytecode::Function>{
      bytecode::Function{
        0, 1, 0, {
          /* var _ = print("Hello, World!"); */
          bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadStringConstant, 0},
          bytecode::ByteCode{bytecode::ByteCodeInstruction::Print, 0},
          bytecode::ByteCode{bytecode::ByteCodeInstruction::SetLocal, 0},

          /* implicit return */
          bytecode::ByteCode{bytecode::ByteCodeInstruction::LoadUndefinedConstant, 0},
          bytecode::ByteCode{bytecode::ByteCodeInstruction::Return, 0},
        }
      }
    },

    std::vector<bytecode::ObjectConstructor>{},

    std::vector<std::int64_t>{ 1, 10, },

    std::vector<double>{},

    std::vector<std::string>{ "Hello, World!\n", "\n" }
  );

  auto vm = std::make_shared<runtime::VirtualMachine>(false, std::cout, std::cin, compiledCode);

  vm->run();

  return 0;
}