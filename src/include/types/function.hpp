#pragma once
#ifndef PARSER_FUNCTION_H
#define PARSER_FUNCTION_H
#include <inttypes.h>
#include <vector>
#include "expr.hpp"

namespace parserTypes
{
  enum funcArgType
  {
    INT,
    CSTR,
    WSTR,
    PTR
  };
  struct funcArg
  {
    funcArgType type;
    struct expr defaultValue;
  };
  struct function
  {
    uint32_t addr;
    std::vector<struct funcArg> args;
  };
}
#endif