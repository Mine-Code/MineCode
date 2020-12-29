#pragma once
#ifndef PARSER_CORE_H
#define PARSER_CORE_H

#include <string>
#include <vector>
#include <sstream>

namespace parserTypes
{
    class condChild;
    class cond;
    class parserContext;
    class ExecFunc;

    class expo;
    class term;
    class expr;
} // namespace parserTypes


namespace parserCore
{
    using Arg=std::pair<std::wstring,std::wstring>;
    using Range=std::pair<int,int>;
    using Context=parserTypes::parserContext;

    int Int  (Context&);
    std::wstring ident(Context&);

    Arg arg(Context&);
    std::wstring attribute(Context&);
    std::wstring ptr(Context&);
    
    std::wstring editable(Context&);
    std::wstring constant(Context&);
    std::wstring value(Context&);
    
    std::wstring power (Context&);
    struct parserTypes::expo expo  (Context&);
    struct parserTypes::term term  (Context&);
    struct parserTypes::expr expr  (Context&);

    struct parserTypes::cond cond  (Context&);
    struct parserTypes::condChild cond_inner  (Context&);

    Range range  (Context&);

    void program(Context&);
    void stmt(parserTypes::parserContext&);
    void func(Context&);
    void If(Context&);
    void For(Context&);
    void While(Context&);
    void put(Context&);
    void assign(Context&);
    struct parserTypes::ExecFunc funcCall(Context&);
} // namespace parserWrap


#endif