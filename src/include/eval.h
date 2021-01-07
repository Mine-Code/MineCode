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
    void expr(parserTypes::expr expr,int dest=13);
} // namespace eval


#endif