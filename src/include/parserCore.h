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
class Cond;

class ExecFunc;

class ptr;

namespace expr {
class expo;
class term;
class Expr;
}  // namespace expr

namespace value {
class BaseValue;
}
namespace stmt {
class BaseStmt;
class BaseFor;
class While;
class If;
class Mcl;
class FuncDef;
class Put;
class Assign;
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
  std::vector<std::wstring> function_list;
  parserWrap *wraper;

  parserWrap *compiler;  // used for compile
  std::unordered_map<std::string, bool>
      puts_table;  // true,false = minecode,asm

  std::wstring funcname = L"global";
  // function

  using Arg = std::pair<std::wstring, std::wstring>;
  using Range = std::pair<int, int>;

  int Int();
  std::wstring ident();

  Arg arg();
  parserTypes::expr::Expr &ptr();

  parserTypes::primary::BasePrimary &editable();
  parserTypes::primary::BasePrimary &constant();
  parserTypes::primary::BasePrimary &value();

  struct parserTypes::primary::BasePrimary &power();
  parserTypes::expr::expo &expo();
  parserTypes::expr::term &term();
  parserTypes::expr::Expr &expr();

  struct parserTypes::Cond cond();
  struct parserTypes::condAnd condAnd();
  struct parserTypes::condChild cond_inner();

  Range range();

  void program();
  parserTypes::stmt::BaseStmt &stmt();
  parserTypes::stmt::FuncDef &func();
  parserTypes::stmt::If &If();
  parserTypes::stmt::BaseFor &For();
  parserTypes::stmt::While &While();
  parserTypes::stmt::Put &put();
  parserTypes::stmt::Assign &assign();
  parserTypes::stmt::Mcl &mcl();

  struct parserTypes::ExecFunc funcCall();
};  // namespace parserWrap

#endif