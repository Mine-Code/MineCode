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
    std::wstring expo  (Context&);
    std::wstring term  (Context&);
    std::wstring expr  (Context&);

    struct cond cond  (Context&);
    struct condChild cond_inner  (Context&);

    Range range  (Context&);

    void program(Context&);
    void stmt(Context&);
    void func(Context&);
    void If(Context&);
    void For(Context&);
    void While(Context&);
    void put(Context&);
    void assign(Context&);
} // namespace parserWrap


#endif