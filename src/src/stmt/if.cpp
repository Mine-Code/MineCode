#include <asm.h>
#include <cond/eval.h>
#include <parserCore.h>

#include <stmt/if.hpp>
parserTypes::stmt::If::~If() {}
void parserTypes::stmt::If::exec(parserCore& ctx) {
  condeval::Cond(&ctx, conditional);

  while (ctx.iter.hasData()) {
    if (ctx.iter.peek() == L"}") break;
    ctx.stmt();
  }
  ctx.Asm->endOfIf();
}