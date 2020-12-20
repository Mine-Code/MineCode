#pragma once
#ifndef STMT_PROC_H
#define STMT_PROC_H

namespace parserCtx
{
    class parserContext;
} // namespace parserCtx

namespace stmtProcessor
{
    void For    (parserCtx::parserContext& ctx);
    void Forr   (parserCtx::parserContext& ctx);
    void While  (parserCtx::parserContext& ctx);
    void If     (parserCtx::parserContext& ctx);
    void Func   (parserCtx::parserContext& ctx);
    void Put    (parserCtx::parserContext& ctx);
    void Assign (parserCtx::parserContext& ctx);
} // namespace stmtProcessor


#endif