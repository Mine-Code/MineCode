#include <mcl.h>
#include <myassert.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <parserWrap.h>
#include <stmtProcessor.h>
#include <syntaxError.h>
#include <util.h>

#include <numeric>

using namespace synErr;
using namespace parserTypes;
using namespace util;

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
    stmt();
  }
}
void parserCore::stmt() {
  // stmt Switcher
  std::wstring text = iter.peek();
  if (text == L"for") {
    For();
    return;
  }
  if (text == L"while") {
    While();
    return;
  }
  if (text == L"if") {
    If();
    return;
  }
  if (text == L"mcl") {
    mcl();
    return;
  }
  if (text == L"return") {
    iter.next();
    Asm->Jump(L"__ret");
    return;
  }
  if (text == L"func") {
    if (iter.peekSafe(1) == L"[") {
      stmtProcessor::executeFunction(this, funcCall());
    } else {
      func();
    }
    return;
  }

  // skip one 'value' and read one
  auto backup = iter.index;
  value();
  text = iter.peek();
  iter.index = backup;
  // done skip and read
  if (text == L"<<") {
    put();
  } else if (text == L"(") {
    stmtProcessor::executeFunction(this, funcCall());
  } else if (isAssignOp(text)) {
    assign();
  }
}
void parserCore::func() {
  assertChar("func");
  std::wstring functionName = iter.next();
  stream << "# "
         << "funcName:" << functionName << std::endl;
  assertChar("(");

  // read arguments
  std::vector<Arg> args;
  // args=(type,name)

  if (iter.peek() != L")") {
    args.emplace_back(arg());
  }
  while (iter.peek() != L")") {
    assertChar(",");
    args.emplace_back(arg());
  }
  // end: read arguments
  assertChar(")");
  assertChar("{");
  stmtProcessor::Func(this);
  assertChar("}");
}
void parserCore::For() {
  assertChar("for");
  std::wstring varname = iter.next();
  assertChar("in");
  stream << "# for ";
  if (iter.peek(1) == L"...") {
    stream << " range" << std::endl;
    Range target = range();
    assertChar("{");
    stmtProcessor::Forr(this, target.first, target.second);
  } else {
    stream << " iter " << std::endl;
    std::wstring target = ident();
    assertChar("{");

    stmtProcessor::For(this, varname, target);
  }
  assertChar("}");
}
void parserCore::put() {
  std::string target = util::wstr2str(ident());
  assertChar("<<");
  // get end
  auto start = iter.index;
  struct expr val = expr();
  auto end = iter.index;
  iter.index = start;
  // get end
  std::wstring expression;
  iter.index = 0;  // enable absolute get
  for (size_t i = start; i < end; i++) {
    expression += iter.peekSafe(i);
  }
  iter.index = end;
  assert(puts.count(target) == 1, L"Puts Not found");
  std::wstring source = util::str2wstr(puts[target]);
  source = convPut(source, expression);
  stream << compiler->compile(source);
}
parserCore::Arg parserCore::arg() {
  return std::make_pair(iter.next(),  // type
                        iter.next()   // name
  );
}
void parserCore::assign() {
  parserTypes::primary::BasePrimary& target = editable();
  std::wstring op = iter.next();
  struct expr value;
  if (!(op == L"++" || op == L"--")) {
    value = expr();
  }
  stmtProcessor::Assign(this, target, op, value);
}

void parserCore::If() {
  assertChar("if");
  struct cond conditional = cond();
  stream << "# if" << std::endl;
  assertChar("{");
  stmtProcessor::If(this, conditional);
  assertChar("}");
  stream << "# fi" << std::endl;
}

void parserCore::While() {
  assertChar("while");
  struct cond conditional = cond();
  stream << "# while {" << std::endl;
  assertChar("{");
  stmtProcessor::While(this, conditional);
  assertChar("}");
  stream << "# }" << std::endl;
}
void parserCore::mcl() {
  assertChar("mcl");
  (*wraper) << util::wstr2str(iter.next());
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

std::wstring convPut(std::wstring src, std::wstring argument) {
  std::wstring ret;
  auto iter = iterator<wchar_t>(util::convToVector<wchar_t, std::wstring>(src));
  while (iter.hasData()) {
    wchar_t ch = iter.next();
    wchar_t ne = iter.peekSafe(1);
    if (ch == '$' && ne == '$') {
      ret += '$';
      iter.next();
    } else if (ch == '$' && iter.peekSafe(0) == 'a' &&
               iter.peekSafe(1) == 'r' && iter.peekSafe(2) == 'g') {
      for (auto ch : argument) {
        ret += ch;
      }
      iter.index += 3;
    } else {
      ret += ch;
    }
  }
  return ret;
}