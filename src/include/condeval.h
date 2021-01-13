#pragma once
#ifndef CONDEVAL_H
#define CONDEVAL_H

namespace parserTypes
{
    class cond;
    class condChild;
} // namespace parserTypes


namespace condeval
{
    void Cond(parserTypes::cond cond);
    void CondChild(parserTypes::condChild cond);
} // namespace condeval


#endif