#include <myassert.h>
#include <parserCore.h>
#include <parserTypes.h>

#include <stmt/all.hpp>

using namespace parserTypes;

std::wstring convPut(std::wstring src, std::wstring argument);

void parserCore::program() {
  if (iter.peekSafe() == L"#") {
    stream << "# do once" << std::endl;
    assert(iter.next() == L"#", L"mismatch preprecessor operation");
    assert(iter.next() == L"do", L"mismatch preprecessor operation");
    assert(iter.next() == L"once", L"mismatch preprecessor operation");
    // TODO: implement do once
  }

  while (iter.hasData()) {
    stmt().exec(*this);
  }
}
struct ExecFunc parserCore::funcCall() {
  ExecFunc ret;
  if (iter.peek() == L"func") {
    // address based call
    ret.type = ExecFunc::ADDRESS;

    assert(iter.next() == L"func", L"excepted 'func'");
    assertChar("[");
    ret.funcAddr = Int();
    assertChar("]");
  } else {
    // name based call
    ret.type = ExecFunc::Name;
    ret.funcId = ident();
  }
  assertChar("(");
  if (iter.peek() != L")") {
    ret.args.emplace_back(expr());
  }
  while (iter.peek() != L")") {
    assertChar(",");
    ret.args.emplace_back(expr());
  }
  assertChar(")");
  return ret;
}
