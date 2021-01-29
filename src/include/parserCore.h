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


namespace parserCore
{
    using Arg=std::pair<std::wstring,std::wstring>;
    using Range=std::pair<int,int>;

    int Int  (parserTypes::parserContext&);
    std::wstring ident(parserTypes::parserContext&);

    Arg arg(parserTypes::parserContext&);
    struct parserTypes::ptr ptr(parserTypes::parserContext&);
    
    struct parserTypes::value editable(parserTypes::parserContext&);
    struct parserTypes::value constant(parserTypes::parserContext&);
    struct parserTypes::value value(parserTypes::parserContext&);
    
    struct parserTypes::power power (parserTypes::parserContext&);
    struct parserTypes::expo expo  (parserTypes::parserContext&);
    struct parserTypes::term term  (parserTypes::parserContext&);
    struct parserTypes::expr expr  (parserTypes::parserContext&);

    struct parserTypes::cond cond  (parserTypes::parserContext&);
    struct parserTypes::condAnd condAnd  (parserTypes::parserContext&);
    struct parserTypes::condChild cond_inner  (parserTypes::parserContext&);

    Range range  (parserTypes::parserContext&);

    void program(parserTypes::parserContext&);
    void stmt(parserTypes::parserContext&);
    void func(parserTypes::parserContext&);
    void If(parserTypes::parserContext&);
    void For(parserTypes::parserContext&);
    void While(parserTypes::parserContext&);
    void put(parserTypes::parserContext&);
    void assign(parserTypes::parserContext&);
    void mcl(parserTypes::parserContext&);

    struct parserTypes::ExecFunc funcCall(parserTypes::parserContext&);
} // namespace parserWrap


#endif