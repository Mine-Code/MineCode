
#pragma once
#ifndef PARSER_EVAL_H
#define PARSER_EVAL_H

#include <memory>
#include <vector>

#include "expo.hpp"
#include "ptr.hpp"
#include "term.hpp"
namespace parserTypes {
struct value {
  enum Type { PTR, IDENT, STR, IMM };
  Type type;

  ptr pointer;
  std::wstring ident;
  std::wstring str;
  uint32_t imm;
};

struct expr {
  std::vector<struct term> parts;
  bool isSingle();
};
struct expo_wrap {
  enum Type { MUL, DIV, MOD };

  Type type;
  struct expo value;
};
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
#endif