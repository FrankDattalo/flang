#ifndef STRING_READER_HPP
#define STRING_READER_HPP

#include "lib.hpp"

#include "Readable.hpp"
#include "Stringable.hpp"

class StringReader : public Readable {
private:
  const std::string file;
  std::size_t index;

public:
  explicit StringReader(std::string  file) noexcept
  : file{std::move(file)}, index{0}
  {}

  ~StringReader() override = default;

  char charAt(std::size_t index) const override;

  void advance(std::size_t amount) override;

  const std::string subString(std::size_t size) const override;

  const std::string getLineFromIndex(std::size_t start) override;

  const std::string toString() const noexcept override;
};

#endif