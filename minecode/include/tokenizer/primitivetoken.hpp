#pragma once

#include <vector>
#include <string>
#include "./basetoken.hpp"

namespace std {
std::string to_string(char c) { return std::string(1, c); }
std::string to_string(const std::string &s) { return s; }
}  // namespace std

namespace minecode::tokenizer {

template <typename T>
class _PrimitiveToken : public BaseToken {
 private:
  const T value;

 public:
  _PrimitiveToken(T value, int line, int column)
      : value(value), BaseToken(line, column) {}
  T GetValue() const { return value; }

  std::string ValueToString() const override { return std::to_string(value); }
};

}  // namespace minecode::tokenizer
