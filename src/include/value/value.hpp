#pragma once

#include <expr/ptr.hpp>
#include <string>
namespace parserTypes {
struct value {
  enum Type { PTR, IDENT, STR, IMM };
  Type type;

  ptr pointer;
  std::wstring ident;
  std::wstring str;
  uint32_t imm;
};
}  // namespace parserTypes