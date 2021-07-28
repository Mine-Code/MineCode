#include <asm.h>
#include <parserCore.h>
#include <util.h>

#include <stmt/funcdef.hpp>
parserTypes::stmt::FuncDef::~FuncDef() {}
void parserTypes::stmt::FuncDef::exec(parserCore& ctx) {
  auto previous_funcname = ctx.funcname;

  ctx.funcname = this->name;
  ctx.Asm->Jump(this->name + L"return");
  ctx.stream << "# Inner Function ";
  ctx.stream << this->name;
  ctx.stream << "\n";

  for (auto&& stmt : this->stmts) {
    stmt->exec(ctx);
  }

  ctx.stream << "# Outer Function\n";
  ctx.Asm->makeLabel(this->name + L"_return");
}
