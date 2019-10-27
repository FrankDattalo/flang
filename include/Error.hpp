#ifndef ERROR_HPP
#define ERROR_HPP

#include "lib.hpp"
#include "Token.hpp"
#include "Readable.hpp"

namespace Error {

  void reportErrorAtToken(
    std::ostream & out,
    const std::string & phase,
    const std::shared_ptr<const Readable>& reader,
    const std::shared_ptr<Token>& token,
    const std::string & message);

  void assertWithPanic(bool mustBeTrue, const std::string & message);
}

#endif
