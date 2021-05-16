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
class parserCore;
namespace condeval
{
    void Cond(parserCore *that, parserTypes::cond cond);
    void Cond(parserCore *that, parserTypes::cond cond, std::wstring trueLabel, std::wstring falseLabel);
    void CondAnd(parserCore *that, parserTypes::condAnd cond, std::wstring target);
    void CondChild(parserCore *that, parserTypes::condChild cond, std::wstring target);
} // namespace condeval

#endif