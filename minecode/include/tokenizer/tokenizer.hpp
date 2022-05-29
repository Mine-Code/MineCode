#pragma once

#include <sstream>
#include <vector>
#include "./basetoken.hpp"
#include "./primitivetoken.hpp"

namespace minecode::tokenizer {
template <typename CharT>
class Tokenizer {
 private:
  std::basic_istringstream<CharT> src;

 public:
  Tokenizer(const std::basic_string<CharT> &src) : src(src) {}

  std::vector<BaseToken *> Tokenize() {
    std::vector<BaseToken *> tokens;
    while (src.good()) {
      auto token = this->GetNextToken();
      if (token) {
        tokens.push_back(token);
      }
    }
    return tokens;
  }

 private:
  BaseToken *GetNextToken() {
    return new _PrimitiveToken<char>(src.get(), 0, 0);
  }
};
}  // namespace minecode::tokenizer
