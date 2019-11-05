#include "lib.hpp"

#include "StringReader.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"
#include "TokenBuffer.hpp"
#include "Parser.hpp"
#include "SemanticAnalyzer.hpp"

enum class FailStep {
  None, Parsing, SemanticAnalysis,
};

FailStep failStepFromString(const std::string & str) {
  if (str == "none") {
    return FailStep::None;
  }

  if (str == "parsing") {
    return FailStep::Parsing;
  }

  if (str == "semantic_analysis") {
    return FailStep::SemanticAnalysis;
  }

  throw std::runtime_error("Unknown fail step provided");
}

FailStep run(const std::string & str) {

  auto reader = std::make_shared<StringReader>(str);
  auto tokenizer = std::make_shared<Tokenizer>(reader);
  auto tokenBuffer = std::make_shared<TokenBuffer>(tokenizer);
  auto parser = std::make_shared<Parser>(tokenBuffer, std::cout);

  auto script = parser->parseScript();

  if (!script) {
    std::cerr << "Failure during parsing!" << std::endl;
    return FailStep::Parsing;
  }

  auto analyzer = std::make_shared<SemanticAnalyzer>(std::cout, reader);

  if (!analyzer->isValid(script.value())) {
    std::cerr << "Failure during semantic analysis!" << std::endl;
    return FailStep::SemanticAnalysis;
  }

  std::cout << "No Failure!" << std::endl;

  return FailStep::None;
}

int main(int argc, char** argv) {

  if (argc != 3) {
    return 1;
  }

  std::string fileName;
  std::string failStepString;

  try {

    fileName = std::string{argv[1]};
    failStepString = std::string(argv[2]);

    std::cout << "Running file: " << fileName << " " << failStepString << std::endl;

    FailStep failStep = failStepFromString(failStepString);

    std::ifstream file{fileName};

    if (file.fail()) {
      std::cerr << "file.fail is true, file may not exist! " << fileName << " " << failStepString << std::endl;
      return 1;
    }

    std::string contents{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};

    FailStep actualFailStep = run(contents);

    if (actualFailStep != failStep) {
      return 1;
    }

  } catch (...) {
    std::cerr << "Caught exception! " << fileName << " " << failStepString << std::endl;

    return 1;
  }

  return 0;
}