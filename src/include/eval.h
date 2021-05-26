#pragma once
#ifndef EVAL_H
#define EVAL_H
#include <string>

namespace parserTypes
{
    class expr;
    class expo;
    class term;
    class power;

    class ptr;
}
class parserCore;
namespace eval
{
    using namespace parserTypes;
    void Expr(parserCore *that, expr val, int dest = 13);
    void Expo(parserCore *that, expo val, int dest = 13);
    void Term(parserCore *that, term val, int dest = 13);
    void Power(parserCore *that, power val, int dest = 13);
    void Ptr(parserCore *that, ptr val, int dest = 13);
    void Var(parserCore *that, std::wstring obj, int dest);
    void Ptr_Addr(parserCore *that, ptr obj, int dest);
} // namespace eval

#endif