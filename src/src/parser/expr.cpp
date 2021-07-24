#include <myassert.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <syntaxError.h>
#include <util.h>

using namespace synErr;
using namespace parserTypes;
using namespace util;

struct value parserCore::value() {
  struct value ret;
  wchar_t ch = iter.peek()[0];
  if (iter.peek() == L"[") {
    ret.type = value::PTR;
    ret.pointer = ptr();
  } else if (isalpha(ch)) {
    ret.type = value::IDENT;
    ret.ident = ident();
  } else if (ch == L'"') {
    ret.type = value::STR;
    ret.str = iter.next();
  } else if (isdigit(ch)) {
    ret.type = value::IMM;
    ret.imm = util::toInt(iter.next());
  } else {
    syntaxError(this, L"is not value type ", __FILE__, __func__, __LINE__);
    throw "";  // do not call this
  }
  return ret;
}
struct ptr parserCore::ptr() {
  assertChar("[");
  struct expr *value = new struct expr;
  *value = expr();
  assertChar("]");
  return parserTypes::ptr(value);
}
struct value parserCore::editable() {
  struct value ret;
  if (iter.peek() == L"[") {
    ret.type = value::PTR;
    ret.pointer = ptr();
  } else {
    ret.type = value::IDENT;
    ret.ident = ident();
  }
  return ret;
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
struct value parserCore::constant() {
  struct value ret;
  std::wstring text = iter.next();
  // check integer?
  if (!isdigit(text[0]) || text[0] != L'"')
    processError(this, L"isn't constant integer", __FILE__, __func__, __LINE__);

  if (isdigit(text[0])) {
    ret.type = value::IMM;
    ret.imm = util::toInt(text);
  } else if (text[0] == '"') {
    ret.type = value::STR;
    ret.str = text;
  }
  return ret;
}
struct power parserCore::power() {
  struct power ret;
  if (iter.peekSafe() == L"(") {
    ret.type = power::EXPR;
    // inner type
    iter.next();
    ret.expr = expr();
    assertChar(")");
  } else if (isFunccall(iter.peekSafe(), iter.peekSafe(1))) {
    ret.type = power::FUNCCALL;
    struct ExecFunc func = funcCall();
    ExecFunc *func2 = new ExecFunc;
    func2->args = func.args;
    func2->funcAddr = func.funcAddr;
    func2->funcId = func.funcId;
    func2->type = func.type;
    ret.func = func2;
  } else if (isInt(iter.peekSafe())) {
    ret.type = power::IMM;
    ret.imm = Int();
  } else if (isSingle(iter.peek()) && iter.peek() != L"[") {
    ret.type = power::VAR;
    ret.var = iter.next();
  } else if (iter.peek() == L"[") {
    ret.type = power::PTR;
    ret.Pointer = ptr();
  } else {
    ret.type = power::EXPR;
    ret.expr = expr();
  }
  return ret;
}
struct expo parserCore::expo() {
  struct expo val;
  val.parts.emplace_back(power());
  while (iter.hasData() && iter.peek() == L"**") {
    assertChar("**");
    val.parts.emplace_back(power());
  }
  return val;
}
struct term parserCore::term() {
  struct term ret;
  {
    expo_wrap wrap;
    wrap.type = expo_wrap::MUL;
    wrap.value = expo();
    ret.parts.emplace_back(wrap);
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
    ret.parts.emplace_back(Elem);
  }
  return ret;
}
struct expr parserCore::expr() {
  struct expr ret;
  struct term part;
  std::wstring text = iter.peek();

  part = term();
  if (text == L"+") {
    iter.next();  // read
  } else if (text == L"-") {
    iter.next();  // read
    struct power pow;
    pow.type = power::IMM;
    pow.imm = -1;

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
      struct power pow;
      pow.type = power::IMM;
      pow.imm = -1;

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