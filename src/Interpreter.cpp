#include "Interpreter.hpp"

std::shared_ptr<ScriptAstNode> parseScript(std::ostream & out, const std::string & data) {
  auto reader = std::make_shared<StringReader>(data);
  auto tokenizer = std::make_shared<Tokenizer>(reader);
  auto tokenBuffer = std::make_shared<TokenBuffer>(tokenizer);
  auto parser = std::make_shared<Parser>(tokenBuffer, out);

  auto maybeScript = parser->parseScript();

  if (!maybeScript) {
    exit(1);
  }

  auto script = maybeScript.value();

  auto analyzer = std::make_shared<SemanticAnalyzer>(out, reader);

  if (!analyzer->isValid(script)) {
    exit(1);
  }

  return script;
}

std::shared_ptr<bytecode::CompiledFile> compile(std::shared_ptr<ScriptAstNode> script) {
  auto compiler = std::make_shared<compiler::AstCompiler>();
  return compiler->compile(script);
}

void interpreter::Interpreter::Run(const std::string & data) {

  std::shared_ptr<ScriptAstNode> script = parseScript(this->out, data);
  auto compiledFile = compile(script);
  auto runtime = std::make_shared<runtime::VirtualMachine>(false, this->out, this->in, compiledFile);
  runtime->run();
}