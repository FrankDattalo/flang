#ifndef IO_HPP
#define IO_HPP

#include "lib.hpp"

namespace io {

std::optional<std::string> readFileToString(const std::string & path) {

  std::ifstream file{path};

  if (file.fail()) {
    return std::nullopt;
  }

  return std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

};

#endif