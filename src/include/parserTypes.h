#pragma once
#ifndef PARSER_CONTEXT_H
#define PARSER_CONTEXT_H

#include <unordered_map>
#include <sstream>
#include "typedIterator.hxx"
#include "asm.h"
namespace parserTypes
{
    enum varType{
        FLOAT,
        INT
    };
    using streamType = std::wstringstream;
    using iterType   = iterator<std::wstring>;
    using varsType   = std::unordered_map<std::string,varType>;
    struct parserContext{
        parserContext();
        Assembly* Asm;
        streamType stream;
        iterType iter;
        varsType variables;
    };

    struct condChild{
        enum Type{
            SINGLE, // var (to val1)

            EQU, // ==
            NEQ, // !=
            LT,  // < 
            GT,  // > 
            GE,  // <=
            LE   // >=
        };
        std::wstring val1;
        Type op;
        std::wstring val2;
    };
    struct cond
    {
        enum Type{
            AND,
            OR
        };

        cond();
        
        condChild first;
        std::vector<std::pair<Type,condChild>> conds;
    };
    struct ExecFunc
    {
        enum CallType{
            ADDRESS,
            Name
        };
        ExecFunc();

        std::vector<std::wstring> args;
        std::wstring funcId;
        CallType type;
    };
    struct expr{
        std::vector<std::wstring> parts;
    };
    struct term{
        std::vector<std::wstring> parts;
    };
} // namespace parserCtx


#endif