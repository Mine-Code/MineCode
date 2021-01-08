#pragma once
#ifndef EVAL_H
#define EVAL_H

namespace parserTypes{
    class parserContext;

    class expr;
    class expo;
    class term;
    class power;
};

namespace eval
{
    using namespace parserTypes;
    void Expr (parserContext& ctx,expr  val,int dest=13);
    void Expo (parserContext& ctx,expo  val,int dest=13);
    void Term (parserContext& ctx,term  val,int dest=13);
    void Power(parserContext& ctx,power val,int dest=13);
} // namespace eval


#endif