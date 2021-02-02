#pragma once
#ifndef PARSER_CONTEXT_H
#define PARSER_CONTEXT_H

#include <unordered_map>
#include <sstream>
#include "typedIterator.hxx"
#include "asm.h"

class parserWrap;

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
    enum funcArgType{
        INT,
        CSTR,
        WSTR,
        PTR
    };
    struct parserContext{
        parserContext();
        parserContext(parserContext&);

        Assembly* Asm;
        streamType stream;
        iterType iter;
        varsType variables;
        std::unordered_map<std::string,std::string> puts;

        parserWrap *wraper;
    };
    
    class value;
    struct ptr{
        value *base;
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
        uint32_t     imm    ;
    };

    struct expr{
        std::vector<struct term> parts;
        bool isSingle();
    };
    struct term{
        std::vector<struct expo_wrap> parts;
        bool isSingle();
    };
    struct expo{
        std::vector<struct power> parts;
        bool isSingle();
    };
    struct expo_wrap{
        enum Type{
            MUL,
            DIV,
            MOD
        };

        Type type;
        struct expo value;
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
        struct ptr ptr;
    };
    
    struct cond
    {
        std::vector<struct condAnd> conds;
    };
    struct condAnd
    {
        std::vector<struct condChild> conds;
    };
    struct condChild{
        enum Type{
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
        enum CallType{
            ADDRESS,
            Name
        };
        ExecFunc();

        std::vector<expr> args;
        std::wstring funcId;
        int funcAddr;
        CallType type;
    };
} // namespace parserCtx


#endif