#pragma once
#ifndef PARSER_CONTEXT_H
#define PARSER_CONTEXT_H

#include <unordered_map>
#include <sstream>
#include <memory>
#include "typedIterator.hxx"
#include "asm.h"
#include "types/varType.hpp"
#include "types/function.hpp"
#include "types/expr.hpp"

class parserWrap;

namespace parserTypes
{
    using streamType = std::wstringstream;
    using iterType = iterator<std::wstring>;
    using varsType = std::unordered_map<std::string, struct varType>;

    struct cond
    {
        std::vector<struct condAnd> conds;
    };
    struct condAnd
    {
        std::vector<struct condChild> conds;
    };
    struct condChild
    {
        enum Type
        {
            COND,

            SINGLE, // var (to val1)
            SINGLE_INV,

            EQU, // ==
            NEQ, // !=
            LT,  // <
            GT,  // >
            GE,  // <=
            LE   // >=
        };

        struct cond child;
        struct value single;

        expr val1;
        Type op;
        expr val2;
    };

    struct ExecFunc
    {
        enum CallType
        {
            ADDRESS,
            Name
        };
        ExecFunc();

        std::vector<expr> args;
        std::wstring funcId;
        int funcAddr;
        CallType type;
    };

    class value;
} // namespace parserCtx

#endif