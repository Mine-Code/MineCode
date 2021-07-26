#include <myassert.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <util.h>

using namespace parserTypes;
using namespace util;

struct cond parserCore::cond() {
  struct cond ret;
  std::wstring text;
  ret.conds.emplace_back(condAnd());
  while (iter.hasData() && (iter.peek() == L"||")) {
    assertChar("||");
    ret.conds.emplace_back(condAnd());
  }
  return ret;
}
struct condAnd parserCore::condAnd() {
  struct condAnd ret;
  ret.conds.emplace_back(cond_inner());
  while (iter.hasData() && (iter.peek() == L"&&")) {
    assertChar("&&");
    ret.conds.emplace_back(cond_inner());
  }
  return ret;
}
struct condChild parserCore::cond_inner() {
  struct condChild cond;
  std::wstring text;

  std::wstring maybeOp;
  {
    auto offs = iter.index;
    value();
    maybeOp = iter.peekSafe();
    iter.index = offs;
  }
  if (isCondOpFull(maybeOp)) {
    cond.val1 = expr();
    std::wstring op = iter.next();
    if (op == L"<") {
      cond.op = condChild::LT;
    } else if (op == L">") {
      cond.op = condChild::GT;
    } else if (op == L"<=") {
      cond.op = condChild::LE;
    } else if (op == L">=") {
      cond.op = condChild::GE;
    } else if (op == L"==") {
      cond.op = condChild::EQU;
    } else if (op == L"!=") {
      cond.op = condChild::NEQ;
    }
    cond.val2 = expr();
  } else {
    cond.op = condChild::SINGLE;
    cond.single = &value();
  }
  return cond;
}