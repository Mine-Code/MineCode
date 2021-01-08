#pragma once
#ifndef PARSER_CONTEXT_H
#define PARSER_CONTEXT_H

#include <unordered_map>
#include <sstream>
#include "typedIterator.hxx"
#include "asm.h"
namespace parserTypes
{
    struct varType{
        enum Type{
            FLOAT,
            INT
        };
        Type type=INT;
        size_t offset=0;
    };
    using streamType = std::wstringstream;
    using iterType   = iterator<std::wstring>;
    using varsType   = std::unordered_map<std::string,struct varType>;
    struct parserContext{
        parserContext();
        Assembly* Asm;
        streamType stream;
        iterType iter;
        varsType variables;
        std::unordered_map<std::string,std::string> puts;
    };

    struct expr{
        std::vector<struct term> parts;
        bool isSingle();
    };
    struct term{
        std::vector<struct expo> parts_mul;
        std::vector<struct expo> parts_div;
        std::vector<struct expo> parts_mod;
        bool isSingle();
    };
    struct expo{
        std::vector<struct power> parts;
        bool isSingle();
    };

    struct power{
        enum Type{
            IMM,
            FLT,
            EXPR,
            FUNCCALL,
            VAR,
            PTR
        };

        power();

        Type type;

        int imm;
        float flt;
        struct expr expr;
        struct ExecFunc* func;
        std::wstring var;
        std::wstring ptr;
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
        std::wstring single;

        expr val1;
        Type op;
        expr val2;
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

        std::vector<expr> args;
        std::wstring funcId;
        struct expr funcAddr;
        CallType type;
    };

    struct ptr;
    struct ptrBase{
        enum Type{
            IMM,
            PTR,
            IDENT
        };
        Type type;

        int imm;
        ptr* pointer;
        std::wstring ident;
    };
    
    class value;
    struct ptr{
        ptr(value);
        value &base;
        std::vector<int> offsets;
    };

    struct value{
        enum Type{
            PTR,
            IDENT,
            STR,
            IMM
        };
        Type type;

        ptr          pointer;
        std::wstring ident  ;
        std::wstring str    ;
        int          imm    ;
    };
} // namespace parserCtx


#endif