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
    void program(parserCtx::parserContext&);
    void stmt(parserCtx::parserContext&);
    void func(parserCtx::parserContext&);
    void For(parserCtx::parserContext&);
    std::wstring arg(parserCtx::parserContext&);
    std::wstring value(parserCtx::parserContext&);
} // namespace parserWrap


#endif