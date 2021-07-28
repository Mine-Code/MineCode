#include <asm.h>
#include <cond/eval.h>
#include <parserCore.h>

#include <stmt/while.hpp>
#include <string>
parserTypes::stmt::While::~While() {}
void parserTypes::stmt::While::exec(parserCore& ctx) {
  int id = ctx.Asm->whileBegin();
  for (auto&& stmt : this->stmts) {
    stmt->exec(ctx);
  }
  std::wstring outerWhile = ctx.Asm->getEnd_While(id);
  condeval::EvalCond(&ctx, conditional, L"", outerWhile);

  ctx.Asm->whileEnd(id);
}