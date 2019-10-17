#include "Reader.hpp"

char Reader::charAt(const std::size_t index) const {
  std::size_t fullIndex = this->index + index;
  if (fullIndex >= this->file.length()) { return -1; }
  return this->file.at(fullIndex);
}

void Reader::advance(const std::size_t amount) {
  this->index += amount;
}

const std::string Reader::toString() const noexcept {
  return (
    "Reader(" 
      "file: " + this->file +
      ", index: " + std::to_string(this->index) +
    ")"
  );
}

const std::string Reader::subString(std::size_t size) const {
  return this->file.substr(this->index, size);
}