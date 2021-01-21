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
    using Context = parserTypes::parserContext;
    using String = std::wstring;

    void For    (Context& ctx,String,String);
    void Forr   (Context& ctx,int,int);
    void While  (Context& ctx,parserTypes::cond conditional);
    void If     (Context& ctx, struct parserTypes::cond conditional);
    void Func   (Context& ctx);
    void Put    (Context& ctx);
    void Assign (Context& ctx,parserTypes::value target,std::wstring op,struct parserTypes::expr& value);
    void executeFunction (Context& ctx,parserTypes::ExecFunc);
} // namespace stmtProcessor


#endif