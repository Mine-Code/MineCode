#pragma once
#ifndef PARSER_VARTYPE_H
#define PARSER_VARTYPE_H
#include <stddef.h>
namespace parserTypes
{
  struct varType
  {
    enum Type
    {
      FLOAT,
      INT
    };
    Type type = INT;
    size_t offset = 0;
  };
}
#endif