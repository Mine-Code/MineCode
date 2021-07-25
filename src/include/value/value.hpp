#pragma once

#include <expr/ptr.hpp>
#include <string>
namespace parserTypes {
namespace value {
class BaseValue {
  BaseValue();
  virtual ~BaseValue();

  virtual void eval(parserCore& ctx, int dest) = 0;
};
// struct value {
//  enum Type { PTR, IDENT, STR, IMM };
//  Type type;
//
//  ptr pointer;
//  std::wstring ident;
//  std::wstring str;
//  uint32_t imm;
//};
}  // namespace value
}  // namespace parserTypes