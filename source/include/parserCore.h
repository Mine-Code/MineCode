#pragma once
#ifndef TOKENPARS_H
#define TOKENPARS_H

#include <string>
#include <vector>
#include <sstream>

namespace parserCtx
{
    class parserContext;
} // namespace parserCtx


namespace parserWrap
{
    std::wstring ident(parserCtx::parserContext&);

    std::wstring arg(parserCtx::parserContext&);
    std::wstring attribute(parserCtx::parserContext&);
    std::wstring ptr(parserCtx::parserContext&);
    
    std::wstring editable(parserCtx::parserContext&);
    std::wstring constant(parserCtx::parserContext&);
    std::wstring value(parserCtx::parserContext&);
    
    void program(parserCtx::parserContext&);
    void stmt(parserCtx::parserContext&);
    void func(parserCtx::parserContext&);
    void For(parserCtx::parserContext&);
    void assign(parserCtx::parserContext&);
} // namespace parserWrap


#endif