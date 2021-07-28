#include <asm.h>
#include <cond/eval.h>
#include <parserCore.h>

#include <stmt/if.hpp>
parserTypes::stmt::If::~If() {}
void parserTypes::stmt::If::exec(parserCore& ctx) {
  ctx.stream << "# if\n";
  condeval::EvalCond(&ctx, conditional);
  std::wcout << this->stmts.size() << std::endl;
  for (auto&& stmt : this->stmts) {
    stmt->exec(ctx);
  }
  ctx.Asm->endOfIf();
  ctx.stream << "# if - end\n";
}