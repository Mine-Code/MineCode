#pragma once

#include <vector>
#include <string>

namespace minecode::tokenizer {
class BaseToken {
 private:
  const int line;
  const int column;

 protected:
  inline BaseToken(int line, int column) : line(line), column(column) {}

 public:
  BaseToken() = delete;

  int GetLine() const { return line; }
  int GetColumn() const { return column; }

  inline std::string ToString() const {
    return this->ValueToString() + "(" + std::to_string(line) + ":" +
           std::to_string(column) + ")";
  }
  virtual std::string ValueToString() const = 0;
};

}  // namespace minecode::tokenizer
