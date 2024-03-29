
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

  struct cond child;
  primary::BasePrimary *single;

  expr val1;
  Type op;
  expr val2;
};
}  // namespace parserTypes