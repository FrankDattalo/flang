#ifndef IO_HPP
#define IO_HPP

#include "lib.hpp"

namespace utils {

std::unique_ptr<std::string> readFileToString(const std::string_view path);

// {

//   std::ifstream file{path};

//   if (file.fail()) {
//     return std::nullopt;
//   }

//   return std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
// }

};

#endif