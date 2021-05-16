#pragma once
#ifndef PARSER_CONTEXT_H
#define PARSER_CONTEXT_H

#include <unordered_map>
#include "typedIterator.hxx"
#include "asm.h"
#include "types/varType.hpp"
#include "types/function.hpp"
#include "types/expr.hpp"
#include "types/cond.hpp"

class parserWrap;

namespace parserTypes
{
    using streamType = std::wstringstream;
    using iterType = iterator<std::wstring>;
    using varsType = std::unordered_map<std::string, struct varType>;

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