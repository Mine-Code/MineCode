#pragma once
#ifndef PARSER_CORE_H
#define PARSER_CORE_H

#include <string>
#include <vector>
#include <sstream>

namespace parserTypes
{
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