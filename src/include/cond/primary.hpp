
#pragma once

#include <inttypes.h>

#include <expr/expr_t.hpp>
#include <primary/primary.hpp>
#include <vector>

#include "or.hpp"
namespace parserTypes {
struct condChild {
  enum Type {
    COND,

    SINGLE,  // var (to val1)
    SINGLE_INV,

    EQU,  // ==
    NEQ,  // !=
    LT,   // <
    GT,   // >
    GE,   // <=
    LE    // >=
  };

  struct Cond child;
  primary::BasePrimary *single;

  expr::Expr val1;
  Type op;
  expr::Expr val2;
};
}  // namespace parserTypes