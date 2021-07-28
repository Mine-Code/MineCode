#include <myassert.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <util.h>

#include <stmt/all.hpp>

using namespace parserTypes;
using namespace util;

stmt::BaseStmt& parserCore::stmt() {
  // stmt Switcher
  std::wstring text = iter.peek();
  if (text == L"for") {
    return For();
  }
  if (text == L"while") {
    return While();
  }
  if (text == L"if") {
    return If();
  }
  if (text == L"mcl") {
    return mcl();
  }
  if (text == L"return") {
    auto ret = new ::stmt::Return;
    return *ret;
  }
  if (text == L"func" && iter.peekSafe(1) != L"[") {
    return func();
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
  auto ret = new ::stmt::Expr;
  ret->val = expr();
  return *ret;
}
stmt::FuncDef& parserCore::func() {
  auto ret = new ::stmt::FuncDef;
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
  return *ret;
}
stmt::BaseFor& parserCore::For() {
  assertChar("for");
  std::wstring varname = iter.next();
  assertChar("in");
  stream << "# for ";
  if (iter.peek(1) == L"...") {
    stream << " range" << std::endl;
    Range target = range();
    auto ret = new ::stmt::ForRange(target.first, target.second);

    assertChar("{");
    while (iter.hasData()) {
      if (iter.peek() == L"}") break;
      ret->stmts.emplace_back(&stmt());
    }
    assertChar("}");

    return *ret;
  } else {
    auto ret = new ::stmt::ForIter();
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
stmt::Put& parserCore::put() {
  auto ret = new ::stmt::Put;

  ret->dest = util::wstr2str(ident());
  assert(puts.count(ret->dest) == 1, L"Puts Not found");

  assertChar("<<");
  size_t start, end;
  {  // get end
    start = iter.index;
    expr();  // skip
    end = iter.index;
    iter.index = start;
  }
  std::wstring expression;
  for (size_t i = 0; i < end - start; i++) {
    expression += iter.next();
  }
  ret->val = expression;
  bool is_minecode = puts_table[ret->dest];
  return *ret;
}
parserCore::Arg parserCore::arg() {
  return std::make_pair(iter.next(),  // type
                        iter.next()   // name
  );
}
stmt::Assign& parserCore::assign() {
  auto ret = new ::stmt::Assign(editable());
  ret->op = iter.next();
  struct Expr value;
  if (!(ret->op == L"++" || ret->op == L"--")) {
    ret->val = expr();
  } else {
    ret->val = *(::Expr*)nullptr;
  }
  return *ret;
}

stmt::If& parserCore::If() {
  auto ret = new ::stmt::If();

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
  auto ret = new ::stmt::While;
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
  return *ret;
}
stmt::Mcl& parserCore::mcl() {
  auto ret = new ::stmt::Mcl();
  assertChar("mcl");
  ret->name = util::wstr2str(iter.next());
  return *ret;
}
