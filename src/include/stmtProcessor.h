#pragma once
#ifndef STMT_PROC_H
#define STMT_PROC_H

namespace parserCtx
{
    class parserContext;
} // namespace parserCtx

namespace stmtProcessor
{
    using Context = parserCtx::parserContext;

    void For    (Context& ctx);
    void Forr   (Context& ctx);
    void While  (Context& ctx);
    void If     (Context& ctx);
    void Func   (Context& ctx);
    void Put    (Context& ctx);
    void Assign (Context& ctx);
} // namespace stmtProcessor


#endif