#pragma once
#ifndef PARSER_CONTEXT_H
#define PARSER_CONTEXT_H

#include <cond/cond.hpp>
#include <unordered_map>

#include "asm.h"
#include "typedIterator.hxx"
#include "types/expr.hpp"
#include "types/function.hpp"
#include "types/varType.hpp"

class parserWrap;

namespace parserTypes {
using streamType = std::wstringstream;
using iterType = iterator<std::wstring>;
using varsType = std::unordered_map<std::string, struct varType>;

struct ExecFunc {
  enum CallType { ADDRESS, Name };
  ExecFunc();

  std::vector<expr> args;
  std::wstring funcId;
  int funcAddr;
  CallType type;
};

class value;
}  // namespace parserTypes

#endif