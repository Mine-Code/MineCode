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

    void For    (Context& ctx,String,String);
    void Forr   (Context& ctx);
    void While  (Context& ctx);
    void If     (Context& ctx);
    void Func   (Context& ctx);
    void Put    (Context& ctx);
    void Assign (Context& ctx);
} // namespace stmtProcessor


#endif