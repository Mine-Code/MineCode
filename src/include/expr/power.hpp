
#pragma once

#include <vector>

#include "expr_t.hpp"
#include "ptr.hpp"
namespace parserTypes {
struct power {
  enum Type { IMM, FLT, EXPR, FUNCCALL, VAR, PTR };

  power();

  Type type;

  int imm;
  float flt;
  struct expr expr;
  struct ExecFunc *func;
  std::wstring var;
  ptr Pointer;
};
}  // namespace parserTypes