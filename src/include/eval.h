#pragma once
#ifndef EVAL_H
#define EVAL_H

namespace parserTypes{
    class expr;
    class expo;
    class power;
};

namespace eval
{
    using namespace parserTypes;
    void Expr (expr  val,int dest=13);
    void Expo (expo  val,int dest=13);
    void Power(power val,int dest=13);
} // namespace eval


#endif