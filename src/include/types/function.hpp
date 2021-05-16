#pragma once
#ifndef PARSER_FUNCTION_H
#define PARSER_FUNCTION_H
#include <stddef.h>
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