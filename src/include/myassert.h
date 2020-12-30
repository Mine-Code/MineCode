#pragma once
#ifndef MYASSERT_H
#define MYASSERT_H

#include <string>
namespace parserTypes
{
    class parserContext;
} // namespace parserTypes


#define assert(cond,msg)\
    if(!(cond))assert_fail(__FILE__,__func__,__LINE__,ctx,msg)

#define assertChar(ch) assert(ctx.iter.next()==L##ch,L"excepted '" ch "'");

void assert_fail(const char*,const char*,int,parserTypes::parserContext&,const wchar_t*);

#endif