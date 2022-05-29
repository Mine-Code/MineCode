#pragma once

#include <vector>
#include <string>
#include "./primitivetoken.hpp"

namespace minecode::tokenizer {
namespace keywords {
enum OtherKeywords {
  kFor,
  kIf,
  kElif,
  kElse,
  kWhile,
  kReturn,
  kTrue,
  kFalse,
  kMcl,
  kLet
};
enum BracketKeywords {
  kParenthesisOpen,
  kParenthesisClose,

  kCurlyBracketOpen,
  kCurlyBracketClose,

  kSquareBracketOpen,
  kSquareBracketClose,
};
enum ExpressionKeywords {
  kPlus,
  kMinus,
  kMultiply,
  kDivide,
  kModulo,
  kPower,

  kBitwiseAnd,
  kBitwiseOr,
  kBitwiseXor,
  kBitwiseNot,
  kBitwiseLeftShift,
  kBitwiseRightShift,

  kBooleanAnd,
  kBooleanOr,
  kBooleanNot,
};
enum ComparisonKeywords {
  kEqual,
  kNotEqual,
  kLessThan,
  kLessThanOrEqual,
  kGreaterThan,
  kGreaterThanOrEqual,
};

}  // namespace keywords

template <typename T>
class _KeywordToken : public _PrimitiveToken<T> {
 private:
  const T keyword;

 public:
  _KeywordToken(T keyword, int line, int column)
      : keyword(keyword), BaseToken(line, column) {}
  T GetValue() const { return keyword; }

  T GetKeyword() const { return keyword; }
  std::string ValueToString() const override {
    return std::to_string(this->keyword);
  }
};

using KeywordKeywordToken = _KeywordToken<keywords::OtherKeywords>;
using BracketKeywordToken = _KeywordToken<keywords::BracketKeywords>;
using ExpressionKeywordToken = _KeywordToken<keywords::ExpressionKeywords>;
using ComparisonKeywordToken = _KeywordToken<keywords::ComparisonKeywords>;

class AssignExpressionKeywordToken : public ExpressionKeywordToken {
 public:
  std::string ValueToString() const override {
    return "Assign(" + std::to_string(this->GetKeyword()) + ")";
  }
};

}  // namespace minecode::tokenizer
