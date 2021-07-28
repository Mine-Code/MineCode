#include <asm.h>
#include <cond/eval.h>
#include <parserCore.h>

#include <stmt/while.hpp>
#include <string>
parserTypes::stmt::While::~While() {}
void parserTypes::stmt::While::exec(parserCore& ctx) {
  int id = ctx.Asm->whileBegin();
  while (ctx.iter.hasData()) {
    if (ctx.iter.peek() == L"}") break;
    ctx.stmt();
  }
  std::wstring outerWhile = ctx.Asm->getEnd_While(id);
  condeval::EvalCond(&ctx, conditional, L"", outerWhile);

  ctx.Asm->whileEnd(id);
}