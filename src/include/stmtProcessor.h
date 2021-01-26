#pragma once
#ifndef STMT_PROC_H
#define STMT_PROC_H

#include <string>

namespace parserTypes
{
    class parserContext;
    class ExecFunc;
    class expr;
    class value;
    class cond;
} // namespace parserTypes

namespace stmtProcessor
{
    using String = std::wstring;

    void For    (parserTypes::parserContext& ctx,String,String);
    void Forr   (parserTypes::parserContext& ctx,int,int);
    void While  (parserTypes::parserContext& ctx,parserTypes::cond conditional);
    void If     (parserTypes::parserContext& ctx, struct parserTypes::cond conditional);
    void Func   (parserTypes::parserContext& ctx);
    void Put    (parserTypes::parserContext& ctx);
    void Assign (parserTypes::parserContext& ctx,parserTypes::value target,std::wstring op,struct parserTypes::expr& value);
    void executeFunction (parserTypes::parserContext& ctx,parserTypes::ExecFunc);
} // namespace stmtProcessor


#endif