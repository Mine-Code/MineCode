#pragma once

#include <sstream>
#include <vector>
#include "./basetoken.hpp"
#include "./primitivetoken.hpp"
#include "./strlitteraltoken.hpp"
#include "keywordtoken.hpp"

namespace minecode::tokenizer {
template <typename CharT>
class Tokenizer {
 private:
  using String = std::basic_string<CharT>;

  std::basic_istringstream<CharT> src;

 public:
  Tokenizer(const String &src) : src(src) {}

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
  void SkipLeadingWhitespace() {
    while (src.good() && (isspace(src.peek()) and src.peek() != '\n')) {
      src.get();
    }
  }

  String ReadIdentifier() {
    String identifier;
    while (src.good() && (isalnum(src.peek()) || src.peek() == '_')) {
      identifier += src.get();
    }
    return identifier;
  }

  int ReadInt() {
    int ret = 0;
    if (src.peek() == '0') {
      src.get();
      auto mode = src.peek();

      if (mode == 'x' || mode == 'X') {
        src.get();
        while (src.good() && isxdigit(src.peek())) {
          ret *= 16;
          if (isdigit(src.peek())) {
            ret += src.get() - '0';
          } else if (isupper(src.peek())) {
            ret += src.get() - 'A' + 10;
          } else {
            ret += src.get() - 'a' + 10;
          }
        }
        return ret;
      } else if (mode == 'o' || mode == 'O') {
        src.get();
        while (src.good() && isdigit(src.peek()) && src.peek() != '8' &&
               src.peek() != '9') {
          ret *= 8;
          ret += src.get() - '0';
        }
        return ret;
      }
    }

    while (src.good() && isdigit(src.peek())) {
      ret = ret * 10 + src.get() - '0';
    }

    return ret;
  }

  String ReadStrLiteral() {
    String ret;
    src.get();
    while (src.good() && src.peek() != '"') {
      ret += src.get();
    }
    src.get();
    return ret;
  }

  BaseToken *GetNextToken() {
    this->SkipLeadingWhitespace();
    if (!src.good()) {
      return nullptr;
    }
    auto c = src.peek();

    if (c == '\n') {
      src.get();
      return new KeywordToken(Keyword::kNewLine, src.tellg(), 0);
    }

    if (isalpha(c)) {
      auto ident = this->ReadIdentifier();

      auto kw = ResolveKeyword(ident);
      if (kw != Keyword::kInvalid) {
        return new KeywordToken(kw, src.tellg(), 0);
      }

      return new _PrimitiveToken<String>(ident, src.tellg(), 0);
    }

    if (isdigit(c)) {
      return new _PrimitiveToken<int>(this->ReadInt(), src.tellg(), 0);
    }

    if (c == '"') {
      return new StrLiteralToken<String>(this->ReadStrLiteral(), src.tellg(),
                                         0);
    }

    if (c == '(') {
      src.get();
      return new KeywordToken(Keyword::kParenthesisOpen, src.tellg(), 0);
    } else if (c == ')') {
      src.get();
      return new KeywordToken(Keyword::kParenthesisClose, src.tellg(), 0);
    } else if (c == '[') {
      src.get();
      return new KeywordToken(Keyword::kSquareBracketOpen, src.tellg(), 0);
    } else if (c == ']') {
      src.get();
      return new KeywordToken(Keyword::kSquareBracketClose, src.tellg(), 0);
    } else if (c == '{') {
      src.get();
      return new KeywordToken(Keyword::kCurlyBracketOpen, src.tellg(), 0);
    } else if (c == '}') {
      src.get();
      return new KeywordToken(Keyword::kCurlyBracketClose, src.tellg(), 0);
    } else if (c == '=') {
      src.get();
      if (src.peek() == '=') {
        src.get();
        return new KeywordToken(Keyword::kEqual, src.tellg(), 0);
      } else {
        return new KeywordToken(Keyword::kAssign, src.tellg(), 0);
      }
    } else if (c == '!') {
      src.get();
      if (src.peek() == '=') {
        src.get();
        return new KeywordToken(Keyword::kNotEqual, src.tellg(), 0);
      } else {
        return new KeywordToken(Keyword::kBooleanNot, src.tellg(), 0);
      }
    } else if (c == '<') {
      src.get();
      c = src.peek();
      Keyword keyword = Keyword::kLessThan;
      if (c == '=') {
        src.get();
        keyword = Keyword::kLessThanOrEqual;
      } else if (c == '<') {
        src.get();
        if (src.peek() == '=') {
          src.get();
          keyword = Keyword::kBitwiseLeftShiftAssign;
        } else {
          keyword = Keyword::kBitwiseLeftShift;
        }
      }
      return new KeywordToken(keyword, src.tellg(), 0);
    } else if (c == '>') {
      src.get();
      c = src.peek();
      Keyword keyword = Keyword::kGreaterThan;
      if (c == '=') {
        src.get();
        keyword = Keyword::kGreaterThanOrEqual;
      } else if (c == '>') {
        src.get();
        if (src.peek() == '=') {
          src.get();
          keyword = Keyword::kBitwiseRightShiftAssign;
        } else {
          keyword = Keyword::kBitwiseRightShift;
        }
      }
      return new KeywordToken(keyword, src.tellg(), 0);
    } else if (c == '|') {
      src.get();
      c = src.peek();
      Keyword keyword = Keyword::kBitwiseOr;
      if (c == '=') {
        src.get();
        keyword = Keyword::kBitwiseOrAssign;
      } else if (c == '|') {
        src.get();
        if (src.peek() == '=') {
          src.get();
          keyword = Keyword::kBooleanOrAssign;
        } else {
          keyword = Keyword::kBooleanOr;
        }
      }
      return new KeywordToken(keyword, src.tellg(), 0);
    } else if (c == '&') {
      src.get();
      c = src.peek();
      Keyword keyword = Keyword::kBitwiseAnd;
      if (c == '=') {
        src.get();
        keyword = Keyword::kBitwiseAndAssign;
      } else if (c == '&') {
        src.get();
        if (src.peek() == '=') {
          src.get();
          keyword = Keyword::kBooleanAndAssign;
        } else {
          keyword = Keyword::kBooleanAnd;
        }
      }
      return new KeywordToken(keyword, src.tellg(), 0);
    } else if (c == '*') {
      src.get();
      c = src.peek();
      Keyword keyword = Keyword::kMultiply;
      if (c == '=') {
        src.get();
        keyword = Keyword::kMultiplyAssign;
      } else if (c == '*') {
        src.get();
        if (src.peek() == '=') {
          src.get();
          keyword = Keyword::kPowerAssign;
        } else {
          keyword = Keyword::kPower;
        }
      }
      return new KeywordToken(keyword, src.tellg(), 0);
    } else if (c == '+') {
      src.get();
      if (src.peek() == '=') {
        src.get();
        return new KeywordToken(Keyword::kPlusAssign, src.tellg(), 0);
      } else {
        return new KeywordToken(Keyword::kPlus, src.tellg(), 0);
      }
    } else if (c == '-') {
      src.get();
      if (src.peek() == '=') {
        src.get();
        return new KeywordToken(Keyword::kMinusAssign, src.tellg(), 0);
      } else {
        return new KeywordToken(Keyword::kMinus, src.tellg(), 0);
      }
    } else if (c == '/') {
      src.get();
      if (src.peek() == '=') {
        src.get();
        return new KeywordToken(Keyword::kDivideAssign, src.tellg(), 0);
      } else {
        return new KeywordToken(Keyword::kDivide, src.tellg(), 0);
      }
    } else if (c == '%') {
      src.get();
      if (src.peek() == '=') {
        src.get();
        return new KeywordToken(Keyword::kModuloAssign, src.tellg(), 0);
      } else {
        return new KeywordToken(Keyword::kModulo, src.tellg(), 0);
      }
    } else if (c == '-') {
      src.get();
      if (src.peek() == '=') {
        src.get();
        return new KeywordToken(Keyword::kMinusAssign, src.tellg(), 0);
      } else {
        return new KeywordToken(Keyword::kMinus, src.tellg(), 0);
      }
    } else if (c == '^') {
      src.get();
      if (src.peek() == '=') {
        src.get();
        return new KeywordToken(Keyword::kBitwiseXorAssign, src.tellg(), 0);
      } else {
        return new KeywordToken(Keyword::kBitwiseXor, src.tellg(), 0);
      }
    } else if (c == '~') {
      src.get();
      if (src.peek() == '=') {
        src.get();
        return new KeywordToken(Keyword::kBitwiseNotAssign, src.tellg(), 0);
      } else {
        return new KeywordToken(Keyword::kBitwiseNot, src.tellg(), 0);
      }
    } else if (c == '.') {
      src.get();

      if (src.peek() != '.') {
        return new KeywordToken(Keyword::kDot, src.tellg(), 0);
      }

      src.get();

      c = src.get();
      if (c != '.') throw std::runtime_error("Unexpected token");

      return new KeywordToken(Keyword::kRange, src.tellg(), 0);
    }

    return new _PrimitiveToken<char>(src.get(), src.tellg(), 0);
  }
};
}  // namespace minecode::tokenizer
