#pragma once

#include <vector>
#include <string>
#include "./primitivetoken.hpp"
#include <map>

namespace minecode::tokenizer {
enum Keyword {
  kFor,
  kIf,
  kElif,
  kElse,
  kWhile,
  kReturn,
  kTrue,
  kFalse,
  kMcl,
  kLet,
  kFunc,
  kRange,
  kIn,

  kParenthesisOpen,
  kParenthesisClose,

  kCurlyBracketOpen,
  kCurlyBracketClose,

  kSquareBracketOpen,
  kSquareBracketClose,

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

  kAssign,

  kPlusAssign,
  kMinusAssign,
  kMultiplyAssign,
  kDivideAssign,
  kModuloAssign,
  kPowerAssign,

  kBitwiseAndAssign,
  kBitwiseOrAssign,
  kBitwiseXorAssign,
  kBitwiseNotAssign,
  kBitwiseLeftShiftAssign,
  kBitwiseRightShiftAssign,

  kBooleanAndAssign,
  kBooleanOrAssign,

  kEqual,
  kNotEqual,
  kLessThan,
  kLessThanOrEqual,
  kGreaterThan,
  kGreaterThanOrEqual,

  kDot,
  kNewLine,
  kInvalid
};

std::map<Keyword, std::string> kKeywordMap = {
    {kFor, "for"},
    {kIf, "if"},
    {kElif, "elif"},
    {kElse, "else"},
    {kWhile, "while"},
    {kReturn, "return"},
    {kTrue, "true"},
    {kFalse, "false"},
    {kMcl, "mcl"},
    {kLet, "let"},
    {kFunc, "func"},
    {kIn, "in"},
    {kRange, "..."},
    {kParenthesisOpen, "("},
    {kParenthesisClose, ")"},
    {kCurlyBracketOpen, "{"},
    {kCurlyBracketClose, "}"},
    {kSquareBracketOpen, "["},
    {kSquareBracketClose, "]"},
    {kPlus, "+"},
    {kMinus, "-"},
    {kMultiply, "*"},
    {kDivide, "/"},
    {kModulo, "%"},
    {kPower, "**"},
    {kBitwiseAnd, "&"},
    {kBitwiseOr, "|"},
    {kBitwiseXor, "^"},
    {kBitwiseNot, "~"},
    {kBitwiseLeftShift, "<<"},
    {kBitwiseRightShift, ">>"},
    {kBooleanAnd, "&&"},
    {kBooleanOr, "||"},
    {kBooleanNot, "!"},
    {kAssign, "="},
    {kPlusAssign, "+="},
    {kMinusAssign, "-="},
    {kMultiplyAssign, "*="},
    {kDivideAssign, "/="},
    {kModuloAssign, "%="},
    {kPowerAssign, "**="},
    {kBitwiseAndAssign, "&="},
    {kBitwiseOrAssign, "|="},
    {kBitwiseXorAssign, "^="},
    {kBitwiseNotAssign, "~="},
    {kBitwiseLeftShiftAssign, "<<="},
    {kBitwiseRightShiftAssign, ">>="},
    {kBooleanAndAssign, "&&="},
    {kBooleanOrAssign, "||="},
    {kEqual, "=="},
    {kNotEqual, "!="},
    {kLessThan, "<"},
    {kLessThanOrEqual, "<="},
    {kGreaterThan, ">"},
    {kGreaterThanOrEqual, ">="},

    {kDot, "."},
    {kNewLine, "\\n"},
    {kInvalid, "<?>"},
};

inline Keyword ResolveKeyword(const std::string &keyword) {
  for (auto &kv : kKeywordMap) {
    if (kv.second == keyword) {
      return kv.first;
    }
  }
  return Keyword::kInvalid;
}

class KeywordToken : public _PrimitiveToken<Keyword> {
 public:
  KeywordToken(Keyword keyword, int line, int column)
      : _PrimitiveToken(keyword, line, column) {}

  std::string ValueToString() const override {
    return "{'" + kKeywordMap[this->GetValue()] + "'}";
  }
};
class AssignExpressionKeywordToken : public KeywordToken {
 public:
  std::string ValueToString() const override {
    return "Assign(" + std::to_string(this->GetValue()) + ")";
  }
};

}  // namespace minecode::tokenizer
