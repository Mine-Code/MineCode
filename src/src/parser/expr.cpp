#include <myassert.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <syntaxError.h>
#include <util.h>

#include <expr/expr.hpp>
#include <primary/all.hpp>

using namespace synErr;
using namespace parserTypes;
using namespace parserTypes::expr;
using namespace util;

parserTypes::primary::BasePrimary& parserCore::value() {
  wchar_t ch = iter.peek()[0];
  if (iter.peek() == L"[") {
    auto ret = new parserTypes::primary::Pointer(ptr());
    return *ret;
  } else if (isalpha(ch)) {
    auto ret = new parserTypes::primary::Variable;
    ret->name = ident();
    return *ret;
  } else if (ch == L'"') {
    auto ret = new parserTypes::primary::Str;
    ret->str = iter.next();
    return *ret;
  } else if (isdigit(ch)) {
    auto ret = new parserTypes::primary::Immutable(util::toInt(iter.next()));
    return *ret;
  } else if (iter.peekSafe(0) == L"func" && iter.peekSafe(1) == L"[") {
    auto ret = new parserTypes::primary::FuncCall();
    auto func = new ExecFunc;
    *func = funcCall();
    ret->func = func;
    return *ret;
  } else {
    syntaxError(this, L"is not value type ", __FILE__, __func__, __LINE__);
    throw "";  // do not call this
  }
}
Expr& parserCore::ptr() {
  assertChar("[");
  auto ret = new Expr;
  expr::*ret = expr();
  assertChar("]");
  return *ret;
}
parserTypes::primary::BasePrimary& parserCore::editable() {
  if (iter.peek() == L"[") {
    auto ret = new parserTypes::primary::Pointer(ptr());
    return *ret;
  } else {
    auto ret = new parserTypes::primary::Variable;
    ret->name = ident();
    return *ret;
  }
}
std::wstring parserCore::ident() {
  std::wstring text = iter.next();
  // check word?
  if (!isalpha(text[0])) {
    processError(this, L"isn't ident", __FILE__, __func__, __LINE__);
  }
  // read under the `.`
  while (iter.peek() == L"." || iter.peek() == L"::") {
    std::wstring joint = iter.next();
    text += joint + iter.next();
  }
  return text;
}
parserTypes::primary::BasePrimary& parserCore::constant() {
  std::wstring text = iter.next();
  // check integer?
  if (!isdigit(text[0]) || text[0] != L'"')
    processError(this, L"isn't constant integer", __FILE__, __func__, __LINE__);

  if (isdigit(text[0])) {
    auto ret = new parserTypes::primary::Immutable(util::toInt(text));
    return *ret;
  } else if (text[0] == '"') {
    auto ret = new parserTypes::primary::Str();
    ret->str = text;
    return *ret;
  } else {
    processError(this, L"unknown error", __FILE__, __func_get, __LINE__);
    throw "";
  }
}
primary::BasePrimary& parserCore::power() {
  if (iter.peekSafe() == L"(") {
    auto ret = new primary::Inner;
    // inner type
    iter.next();
    ret->expr = expr();
    assertChar(")");
    return *ret;
  } else if (isFunccall(iter.peekSafe(), iter.peekSafe(1))) {
    auto ret = new primary::FuncCall;
    struct ExecFunc func = funcCall();
    ExecFunc* func2 = new ExecFunc;
    func2->args = func.args;
    func2->funcAddr = func.funcAddr;
    func2->funcId = func.funcId;
    func2->type = func.type;
    ret->func = func2;
    return *ret;
  } else if (isInt(iter.peekSafe())) {
    auto ret = new primary::Immutable(Int());
    return *ret;
  } else if (isSingle(iter.peek()) && iter.peek() != L"[") {
    auto ret = new primary::Variable;
    ret->name = iter.next();
    return *ret;
  } else if (iter.peek() == L"[") {
    auto ret = new primary::Pointer(ptr());
    return *ret;
  } else {
    auto ret = new primary::Inner;
    ret->expr = expr();
    return *ret;
  }
}
expo parserCore::expo() {
  struct expo val;
  val.parts.emplace_back(&power());
  while (iter.hasData() && iter.peek() == L"**") {
    assertChar("**");
    val.parts.emplace_back(&power());
  }
  return val;
}
term& parserCore::term() {
  auto ret = new expr::term;
  {
    expo_wrap wrap;
    wrap.type = expo_wrap::MUL;
    wrap.value = expo();
    ret->parts.emplace_back(wrap);
  }
  while (iter.hasData() &&
         (iter.peek() == L"*" || iter.peek() == L"/" || iter.peek() == L"%")) {
    auto text = iter.next();
    assert(text == L"*" || text == L"/" || text == L"%",
           L"excepted '*' or '/','%'");

    expo_wrap Elem;
    if (text == L"*") {
      Elem.type = expo_wrap::MUL;
    } else if (text == L"/") {
      Elem.type = expo_wrap::DIV;
    } else if (text == L"%") {
      Elem.type = expo_wrap::MOD;
    }
    Elem.value = expo();
    ret->parts.emplace_back(Elem);
  }
  return *ret;
}
Expr& parserCore::expr() {
  struct Expr ret;
  struct term part;
  std::wstring text = iter.peek();

  part = term();
  if (text == L"+") {
    iter.next();  // read
  } else if (text == L"-") {
    iter.next();  // read
    auto pow = new primary::Immutable(-1);

    struct expo tmp;
    tmp.parts.emplace_back(pow);

    struct expo_wrap wrap;
    wrap.value = tmp;
    wrap.type = expo_wrap::MUL;

    part.parts.emplace_back(wrap);
  }
  ret.parts.emplace_back(part);
  while (iter.hasData() && (iter.peek() == L"+" || iter.peek() == L"-" ||
                            isBitOpFull(iter.peek()))) {
    auto text = iter.next();
    assert(text == L"+" || text == L"-" || isBitOpFull(text),
           L"excepted '+' or '+', bitOperator");

    part = term();
    if (text == L"-") {
      auto pow = new primary::Immutable(-1);

      struct expo tmp;
      tmp.parts.emplace_back(pow);

      struct expo_wrap wrap;
      wrap.value = tmp;
      wrap.type = expo_wrap::MUL;

      part.parts.emplace_back(wrap);
    }
    ret.parts.emplace_back(part);
  }
  return ret;
}
parserCore::Range parserCore::range() {
  int start = Int();
  assertChar("...");
  int end = Int();
  // convert start/end: wstr => int
  return std::make_pair(start, end);
}
int parserCore::Int() {
  std::wstring text = iter.next();
  if (!isdigit(text[0])) {
    syntaxError(this, L"is not integer", __FILE__, __func__, __LINE__);
  }
  // convert test<wstr> to value<int>
  return toInt(text);
}