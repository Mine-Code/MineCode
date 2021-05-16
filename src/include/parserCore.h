#pragma once
#ifndef PARSER_CORE_H
#define PARSER_CORE_H

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <typedIterator.hxx>

namespace parserTypes
{
    class varType;
    class function;
    class condChild;
    class condAnd;
    class cond;

    class parserContext;
    class ExecFunc;

    class value;
    class ptr;

    class power;
    class expo;
    class term;
    class expr;
} // namespace parserTypes

class parserCore
{
public:
    using streamType = std::wstringstream;
    using iterType = iterator<std::wstring>;
    using varsType = std::unordered_map<std::string, parserTypes::varType>;
    // variables
    Assembly *Asm;
    streamType stream;
    iterType iter;
    varsType variables;
    std::unordered_map<std::string, std::string> puts;
    std::unordered_map<std::string, parserTypes::function> functions;
    parserWrap *wraper;

    parserWrap *compiler;                             // used for compile
    std::unordered_map<std::string, bool> puts_table; // true,false = minecode,asm

    // function

    using Arg = std::pair<std::wstring, std::wstring>;
    using Range = std::pair<int, int>;

    int Int();
    std::wstring ident();

    Arg arg();
    struct parserTypes::ptr ptr();

    struct parserTypes::value editable();
    struct parserTypes::value constant();
    struct parserTypes::value value();

    struct parserTypes::power power();
    struct parserTypes::expo expo();
    struct parserTypes::term term();
    struct parserTypes::expr expr();

    struct parserTypes::cond cond();
    struct parserTypes::condAnd condAnd();
    struct parserTypes::condChild cond_inner();

    Range range();

    void program();
    void stmt();
    void func();
    void If();
    void For();
    void While();
    void put();
    void assign();
    void mcl();

    struct parserTypes::ExecFunc funcCall();
}; // namespace parserWrap

#endif