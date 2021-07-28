#include <myassert.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <parserWrap.h>
#include <stmtProcessor.h>
#include <syntaxError.h>
#include <util.h>

#include <numeric>
#include <stmt/all.hpp>

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
stmt::BaseStmt& parserCore::stmt() {
  // stmt Switcher
  std::wstring text = iter.peek();
  if (text == L"for") {
    return (stmt::BaseStmt&)For();
  }
  if (text == L"while") {
    return (stmt::BaseStmt&)While();
  }
  if (text == L"if") {
    return (stmt::BaseStmt&)If();
  }
  if (text == L"mcl") {
    return (stmt::BaseStmt&)mcl();
  }
  if (text == L"return") {
    auto ret = (new stmt::Return);
    return (stmt::BaseStmt&)*ret;
  }
  if (text == L"func" && iter.peekSafe(1) != L"[") {
    return (stmt::BaseStmt&)func();
  }

  // skip one 'value' and read one
  auto backup = iter.index;
  value();
  text = iter.peek();
  iter.index = backup;
  // done skip and read
  if (text == L"<<") {
    return put();
  } else if (isAssignOp(text)) {
    return assign();
  }

  // default = expr
  auto ret = new parserTypes::stmt::Expr;
  ret->val = expr();
  return *ret;
}
stmt::FuncDef& parserCore::func() {
  auto ret = new stmt::FuncDef;
  assertChar("func");
  ret->name = iter.next();
  stream << "# "
         << "funcName:" << ret->name << std::endl;
  assertChar("(");

  // read arguments
  std::vector<Arg> args;
  // args=(type,name)

  if (iter.peek() != L")") {
    ret->args.emplace_back(arg());
  }
  while (iter.peek() != L")") {
    assertChar(",");
    ret->args.emplace_back(arg());
  }
  // end: read arguments
  assertChar(")");
  assertChar("{");
  while (iter.hasData()) {
    if (iter.peek() == L"}") break;
    ret->stmts.emplace_back(&stmt());
  }
  assertChar("}");
}
stmt::BaseFor& parserCore::For() {
  assertChar("for");
  std::wstring varname = iter.next();
  assertChar("in");
  stream << "# for ";
  if (iter.peek(1) == L"...") {
    stream << " range" << std::endl;
    Range target = range();
    auto ret = new stmt::ForRange(target.first, target.second);

    assertChar("{");
    while (iter.hasData()) {
      if (iter.peek() == L"}") break;
      ret->stmts.emplace_back(&stmt());
    }
    assertChar("}");

    return *ret;
  } else {
    auto ret = new stmt::ForIter();
    ret->target = varname;
    ret->iter = expr();

    stream << " iter " << std::endl;

    assertChar("{");
    while (iter.hasData()) {
      if (iter.peek() == L"}") break;
      ret->stmts.emplace_back(&stmt());
    }
    assertChar("}");

    return *ret;
  }
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
  bool is_minecode = puts_table[target];
  std::wstring source = util::str2wstr(puts[target]);
  source = convPut(source, expression);
  if (is_minecode) {
    stream << compiler->compile(source);
  } else {
    stream << source;
  }
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

parserTypes::stmt::If& parserCore::If() {
  auto ret = new parserTypes::stmt::If();

  assertChar("if");
  ret->conditional = cond();
  stream << "# if" << std::endl;
  assertChar("{");
  while (iter.hasData()) {
    if (iter.peek() == L"}") break;
    ret->stmts.emplace_back(&stmt());
  }
  assertChar("}");
  stream << "# fi" << std::endl;
  return *ret;
}

stmt::While& parserCore::While() {
  auto ret = new stmt::While;
  assertChar("while");
  ret->conditional = cond();
  stream << "# while {" << std::endl;
  assertChar("{");
  while (iter.hasData()) {
    if (iter.peek() == L"}") break;
    ret->stmts.emplace_back(&stmt());
  }
  assertChar("}");
  stream << "# }" << std::endl;
}
stmt::Mcl& parserCore::mcl() {
  auto ret = new stmt::Mcl();
  assertChar("mcl");
  ret->name = util::wstr2str(iter.next());
  return *ret;
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