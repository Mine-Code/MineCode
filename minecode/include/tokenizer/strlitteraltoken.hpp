#pragma once

#include <vector>
#include <string>
#include "./basetoken.hpp"

namespace minecode::tokenizer {

template <typename String>
class StrLiteralToken : public BaseToken {
 private:
  const String value;

 public:
  StrLiteralToken(String value, int line, int column)
      : value(value), BaseToken(line, column) {}
  String GetValue() const { return value; }

  std::string ValueToString() const override {
    return "\"" + std::to_string(value) + "\"";
  }
};

}  // namespace minecode::tokenizer
