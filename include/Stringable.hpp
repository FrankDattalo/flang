#ifndef STRINGABLE_HPP
#define STRINGABLE_HPP

#include "lib.hpp"

class Stringable {
public:
  virtual const std::string toString() const noexcept = 0;
};

#endif // STRINGABLE_HPP 