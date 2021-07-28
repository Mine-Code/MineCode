#include <asm.h>
#include <parserCore.h>

#include <stmt/funcdef.hpp>
parserTypes::stmt::FuncDef::~FuncDef() {}
void parserTypes::stmt::FuncDef::exec(parserCore& ctx) {
  auto previous_funcname = ctx.funcname;

  ctx.funcname = this->name;

  ctx.stream << "# Inner Function\n";
  while (ctx.iter.hasData()) {
    if (ctx.iter.peek() == L"}") break;
    ctx.stmt();
  }
  ctx.stream << "# Outer Function\n";
  ctx.Asm->makeLabel(this->name + L"_return");
}
