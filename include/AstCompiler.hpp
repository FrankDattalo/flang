#ifndef ASTCOMPILER_HPP
#define ASTCOMPILER_HPP

#include "lib.hpp"
#include "Ast.hpp"
#include "AstWalker.hpp"
#include "ByteCode.hpp"

namespace compiler {

class AstCompiler {
private:

public:

  std::shared_ptr<bytecode::CompiledFile> compile(std::shared_ptr<ScriptAstNode> file) noexcept;

};

};

#endif
