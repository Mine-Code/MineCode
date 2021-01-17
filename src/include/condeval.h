#pragma once
#ifndef CONDEVAL_H
#define CONDEVAL_H

#include <string>

namespace parserTypes
{
    class parserContext;
    class cond;
    class condChild;
} // namespace parserTypes


namespace condeval
{
    void Cond(parserTypes::parserContext ctx, parserTypes::cond cond);
    void CondChild(parserTypes::parserContext ctx, parserTypes::condChild cond, std::wstring target);
} // namespace condeval


#endif