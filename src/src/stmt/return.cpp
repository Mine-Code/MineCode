
#include <asm.h>
#include <parserCore.h>

#include <stmt/return.hpp>
parserTypes::stmt::Return::~Return() {}
void parserTypes::stmt::Return::exec(parserCore &ctx) {
  ctx.Asm->Jump(ctx.funcname + L"_return");
}