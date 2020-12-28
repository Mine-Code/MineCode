#pragma once
#ifndef STMT_PROC_H
#define STMT_PROC_H

#include <string>

namespace parserTypes
{
    class parserContext;
} // namespace parserTypes

namespace stmtProcessor
{
    using Context = parserTypes::parserContext;
    using String = std::wstring;

    void For    (parserTypes::parserContext& ctx,String,String);
    void Forr   (parserTypes::parserContext& ctx,int,int);
    void While  (parserTypes::parserContext& ctx);
    void If     (parserTypes::parserContext& ctx);
    void Func   (parserTypes::parserContext& ctx);
    void Put    (parserTypes::parserContext& ctx);
    void Assign (parserTypes::parserContext& ctx);
} // namespace stmtProcessor


#endif