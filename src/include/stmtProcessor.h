#pragma once
#ifndef STMT_PROC_H
#define STMT_PROC_H

#include <string>

namespace parserCtx
{
    class parserContext;
} // namespace parserCtx

namespace stmtProcessor
{
    using Context = parserCtx::parserContext;
    using String = std::wstring;

    void For    (parserCtx::parserContext& ctx,String,String);
    void Forr   (parserCtx::parserContext& ctx,int,int);
    void While  (parserCtx::parserContext& ctx);
    void If     (parserCtx::parserContext& ctx);
    void Func   (parserCtx::parserContext& ctx);
    void Put    (parserCtx::parserContext& ctx);
    void Assign (parserCtx::parserContext& ctx);
} // namespace stmtProcessor


#endif