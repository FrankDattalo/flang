#ifndef READABLE_HPP
#define READABLE_HPP

#include "lib.hpp"

class Readable {
public:
  explicit Readable() noexcept = default;

  virtual ~Readable() noexcept = default;

  virtual char charAt(std::size_t index) const = 0;

  virtual void advance(std::size_t amount) = 0;

  virtual const std::string getLineFromIndex(std::size_t start) const = 0;

  virtual const std::string subString(std::size_t size) const = 0;
};

#endif // READABLE_HPP