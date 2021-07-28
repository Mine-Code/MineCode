#pragma once
#ifndef EVAL_H
#define EVAL_H
#include <string>

namespace parserTypes {
class Expr;
class expo;
class term;

class ptr;
namespace primary {
class BasePrimary;
}
}  // namespace parserTypes
class parserCore;
namespace eval {
void Expr(parserCore *that, parserTypes::Expr val, int dest = 13);
void Expo(parserCore *that, parserTypes::expo val, int dest = 13);
void Term(parserCore *that, parserTypes::term val, int dest = 13);
void Power(parserCore *that, parserTypes::primary::BasePrimary &val,
           int dest = 13);
void Ptr(parserCore *that, parserTypes::ptr val, int dest = 13);
void Var(parserCore *that, std::wstring obj, int dest);
void Ptr_Addr(parserCore *that, parserTypes::ptr obj, int dest);
}  // namespace eval

#endif