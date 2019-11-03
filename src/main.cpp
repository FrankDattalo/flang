#include "lib.hpp"
#include "Io.hpp"
#include "Interpreter.hpp"

void usage() {
  std::cerr << "This is where I'm going to print how to use this" << std::endl;
  exit(1);
}

int main(int argc, char** argv) {

  if (argc != 2) {
    usage();
    return 1;
  }

  std::string filePath{argv[1]};

  auto contents = io::readFileToString(filePath);

  if (!contents) {
    usage();
    return 1;
  }

  auto interpreter = std::make_shared<interpreter::Interpreter>(std::cout, std::cin);
  interpreter->Run(contents.value());

  return 0;
}