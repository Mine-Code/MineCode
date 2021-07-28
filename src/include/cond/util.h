#pragma once
#ifndef CONDUTIL_H
#define CONDUTIL_H

namespace parserTypes {
class Cond;
class condAnd;
class condChild;
}  // namespace parserTypes

namespace util {
using namespace parserTypes;

condAnd cond2condAnd(Cond source);
Cond condAnd2cond(condAnd source);

condChild invertConditional(condChild source);
condAnd invertConditional(Cond source);
Cond invertConditional(condAnd source);
}  // namespace util

#endif