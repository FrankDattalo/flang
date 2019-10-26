#include "Error.hpp"

void ErrorUtils::reportErrorAtToken(
    std::ostream & out,
    const std::string & phase,
    const std::shared_ptr<Readable>& reader,
    const std::shared_ptr<Token>& token,
    const std::string & message) {

  if (token->tokenType == TokenType::EndOfFile) {
    out
      << "At end of file an error was discovered during " << phase << " phase:" << std::endl
      << message << std::endl << std::endl;

  } else {
    std::size_t startofLine = token->sourceIndex - (token->sourceColumn - 1);
    auto line = reader->getLineFromIndex(startofLine);

    std::string arrow;

    for (std::size_t i = 1; i < token->sourceColumn; i++) {
      arrow.append("~");
    }

    arrow.append("^");

    out
      << "On line " << token->sourceLine
      << " at column " << token->sourceColumn
      << " within source index at " << token->sourceIndex
      << " an error was discovered during " << phase << " phase:" << std::endl
      << message << std::endl
      << line << std::endl
      << arrow << std::endl << std::endl;
  }
}

void panic(const std::string& message) {
  std::cerr << message << std::endl;
  exit(1);
}