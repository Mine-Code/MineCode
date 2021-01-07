#pragma once
#ifndef EVAL_H
#define EVAL_H

#include <parserTypes.h>

namespace eval
{
    void expr(parserTypes::expr expr,int dest=13);
} // namespace eval


#endif