
#pragma once
#ifndef PARSER_COND_H
#define PARSER_COND_H

#include <inttypes.h>
#include <vector>
#include "expr.hpp"

namespace parserTypes
{
  struct cond
  {
    std::vector<struct condAnd> conds;
  };
  struct condAnd
  {
    std::vector<struct condChild> conds;
  };
  struct condChild
  {
    enum Type
    {
      COND,

      SINGLE, // var (to val1)
      SINGLE_INV,

      EQU, // ==
      NEQ, // !=
      LT,  // <
      GT,  // >
      GE,  // <=
      LE   // >=
    };

    struct cond child;
    struct value single;

    expr val1;
    Type op;
    expr val2;
  };
}
#endif