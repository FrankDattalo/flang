#ifndef PARSER_HPP
#define PARSER_HPP

#include "lib.hpp"
#include "Stringable.hpp"
#include "Ast.hpp"

class Parser : public Stringable {
private:

public:

  ~Parser() noexcept override = default;

   const std::string toString() const noexcept override;
};

#endif
