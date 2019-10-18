#include "StringReader.hpp"

char StringReader::charAt(const std::size_t index) const {
  std::size_t fullIndex = this->index + index;
  if (fullIndex >= this->file.length()) { return '\0'; }
  return this->file.at(fullIndex);
}

void StringReader::advance(const std::size_t amount) {
  this->index += amount;
}

const std::string StringReader::toString() const noexcept {
  return (
    "StringReader("
      "file: " + this->file +
      ", index: " + std::to_string(this->index) +
    ")"
  );
}

const std::string StringReader::subString(std::size_t size) const {
  return this->file.substr(this->index, size);
}