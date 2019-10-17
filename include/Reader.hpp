#ifndef READER_HPP
#define READER_HPP

#include "lib.hpp"

#include "Stringable.hpp"

class Reader : Stringable {
private:
  const std::string file;
  std::size_t index;
  
public:
  explicit Reader(std::string  file) noexcept 
  : file{std::move(file)}, index{0} 
  {}

  virtual ~Reader() = default;

  char charAt(std::size_t index) const;

  void advance(std::size_t amount);

  const std::string subString(std::size_t size) const;

  const std::string toString() const noexcept override;
};

#endif // READER_HPP