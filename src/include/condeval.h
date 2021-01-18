#pragma once
#ifndef CONDEVAL_H
#define CONDEVAL_H

#include <string>

namespace parserTypes
{
    class parserContext;
    class cond;
    class condAnd;
    class condChild;
} // namespace parserTypes


namespace condeval
{
    void Cond(parserTypes::parserContext ctx, parserTypes::cond cond);
    void CondAnd(parserTypes::parserContext ctx, parserTypes::condAnd cond, std::wstring target);
    void CondChild(parserTypes::parserContext ctx, parserTypes::condChild cond, std::wstring target);
} // namespace condeval


#endif