#pragma once

#include "./basetoken.hpp"
#include <sstream>
#include <vector>

namespace minecode::tokenizer {
template <typename CharT>
class Tokenizer {
 private:
  std::basic_stringstream<CharT> src;

 private:
  Tokenizer(const std::basic_string<CharT> &src) : src(src) {}

  BaseToken *GetNextToken() {
    while (src.peek() != EOF) {
      if (src.peek() == ' ' || src.peek() == '\t' || src.peek() == '\n') {
        char d;
        src >> d;
        continue;
      }
    }
  }

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
};
}  // namespace minecode::tokenizer
