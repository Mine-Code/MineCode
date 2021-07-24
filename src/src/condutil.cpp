#include <condutil.h>
#include <parserTypes.h>

using namespace parserTypes;

condChild util::invertConditional(condChild source) {
  condChild::Type old = source.op;
  switch (old) {
    case condChild::SINGLE:
      source.op = condChild::SINGLE_INV;
      break;
    case condChild::SINGLE_INV:
      source.op = condChild::SINGLE;
      break;
    case condChild::EQU:
      source.op = condChild::NEQ;
      break;
    case condChild::NEQ:
      source.op = condChild::EQU;
      break;
    case condChild::LT:
      source.op = condChild::GE;
      break;
    case condChild::GT:
      source.op = condChild::LE;
      break;
    case condChild::GE:
      source.op = condChild::LT;
      break;
    case condChild::LE:
      source.op = condChild::GT;
      break;
    case condChild::COND:
      source.child = condAnd2cond(invertConditional(source.child));
      break;
  }
  return source;
}
condAnd util::invertConditional(cond source) {
  condAnd ret;
  for (auto part : source.conds) {
    condChild wrap;
    wrap.op = condChild::COND;
    wrap.child = invertConditional(part);

    ret.conds.emplace_back(wrap);
  }
  return ret;
}
cond util::invertConditional(condAnd source) {
  cond ret;
  for (auto part : source.conds) {
    condAnd wrap;
    wrap.conds.emplace_back(invertConditional(part));
    ret.conds.emplace_back(wrap);
  }

  return ret;
}

condAnd util::cond2condAnd(cond source) {
  condChild wrap;
  wrap.op = condChild::COND;
  wrap.child = source;

  condAnd ret;
  ret.conds.emplace_back(wrap);

  return ret;
}
cond util::condAnd2cond(condAnd source) {
  cond ret;
  ret.conds.emplace_back(source);

  return ret;
}