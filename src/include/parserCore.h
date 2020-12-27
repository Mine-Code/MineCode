#pragma once
#ifndef PARSER_CORE_H
#define PARSER_CORE_H

#include <string>
#include <vector>
#include <sstream>

namespace parserCtx
{
    class parserContext;
} // namespace parserCtx


namespace parserCore
{
    using Arg=std::pair<std::wstring,std::wstring>;
    using Range=std::pair<int,int>;

    int Int  (parserCtx::parserContext&);
    std::wstring ident(parserCtx::parserContext&);

    Arg arg(parserCtx::parserContext&);
    std::wstring attribute(parserCtx::parserContext&);
    std::wstring ptr(parserCtx::parserContext&);
    
    std::wstring editable(parserCtx::parserContext&);
    std::wstring constant(parserCtx::parserContext&);
    std::wstring value(parserCtx::parserContext&);
    
    std::wstring power (parserCtx::parserContext&);
    std::wstring expo  (parserCtx::parserContext&);
    std::wstring term  (parserCtx::parserContext&);
    std::wstring expr  (parserCtx::parserContext&);

    std::wstring cond  (parserCtx::parserContext&);
    std::wstring cond_inner  (parserCtx::parserContext&);

    Range range  (parserCtx::parserContext&);

    void program(parserCtx::parserContext&);
    void stmt(parserCtx::parserContext&);
    void func(parserCtx::parserContext&);
    void If(parserCtx::parserContext&);
    void For(parserCtx::parserContext&);
    void put(parserCtx::parserContext&);
    void assign(parserCtx::parserContext&);
} // namespace parserWrap


#endif