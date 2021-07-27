#pragma once
#ifndef PARSER_CORE_H
#define PARSER_CORE_H

#include <primary/primary.hpp>
#include <sstream>
#include <string>
#include <typedIterator.hxx>
#include <unordered_map>
#include <vector>

#include "types/function.hpp"
#include "types/varType.hpp"

class Assembly;
class parserWrap;

namespace parserTypes {
class function;
class condChild;
class condAnd;
class cond;

class ExecFunc;

class ptr;

class expo;
class term;
class expr;

namespace value {
class BaseValue;
}
namespace stmt {
class BaseStmt;
class BaseFor;
}  // namespace stmt
}  // namespace parserTypes

class parserCore {
 public:
  using streamType = std::wstringstream;
  using iterType = iterator<std::wstring>;
  using varsType = std::unordered_map<std::string, parserTypes::varType>;
  // variables
  Assembly *Asm;
  streamType stream;
  iterType iter;
  varsType variables;
  std::unordered_map<std::string, std::string> puts;
  std::unordered_map<std::string, parserTypes::function> functions;
  parserWrap *wraper;

  parserWrap *compiler;  // used for compile
  std::unordered_map<std::string, bool>
      puts_table;  // true,false = minecode,asm

  // function

  using Arg = std::pair<std::wstring, std::wstring>;
  using Range = std::pair<int, int>;

  int Int();
  std::wstring ident();

  Arg arg();
  struct parserTypes::ptr ptr();

  parserTypes::primary::BasePrimary &editable();
  parserTypes::primary::BasePrimary &constant();
  parserTypes::primary::BasePrimary &value();

  struct parserTypes::primary::BasePrimary &power();
  struct parserTypes::expo expo();
  struct parserTypes::term term();
  struct parserTypes::expr expr();

  struct parserTypes::cond cond();
  struct parserTypes::condAnd condAnd();
  struct parserTypes::condChild cond_inner();

  Range range();

  void program();
  parserTypes::stmt::BaseStmt &stmt();
  void func();
  void If();
  parserTypes::stmt::BaseFor &For();
  void While();
  void put();
  void assign();
  void mcl();

  struct parserTypes::ExecFunc funcCall();
};  // namespace parserWrap

#endif