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