#ifndef ERROR_HPP
#define ERROR_HPP

#include "lib.hpp"

namespace utils {

  void AssertWithPanic(bool mustBeTrue, const std::string & message);

}

#endif
