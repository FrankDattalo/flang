#include "lib.hpp"
#include "Io.hpp"
#include "Interpreter.hpp"

void usage(const std::string & msg) {
  std::cerr
    << "Error: " << msg << '\n'
    << "Usage: flang <path to source code file>" << std::endl;

  exit(1);
}

int main(int argc, char** argv) {

  if (argc != 2) {
    usage("Unexpected number of arguments.");
    return 1;
  }

  std::string filePath{argv[1]};

  auto contents = io::readFileToString(filePath);

  if (!contents) {
    usage("Could not open file.");
    return 1;
  }

  auto interpreter = std::make_shared<interpreter::Interpreter>(std::cout, std::cin);
  interpreter->Run(contents.value());

  return 0;
}