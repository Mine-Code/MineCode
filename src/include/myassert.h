#pragma once
#ifndef MYASSERT_H
#define MYASSERT_H

#include <string>
namespace parserTypes
{
    class parserContext;
} // namespace parserTypes
#ifdef __USE_GNU
#define __func_get __PRETTY_FUNCTION__
#else
#define __func_get __FUNCTION__
#endif

#define assert(cond, msg) \
    if (!(cond))          \
    assert_fail(__FILE__, __func_get, __LINE__, ctx, msg)

#define assertChar(ch) assert(iter.next() == L##ch, L"excepted '" ch "'");

void assert_fail(const char *, const char *, int, parserTypes::parserContext &, const wchar_t *);

#endif