#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>
using namespace parserTypes;
#include <expr/expr_t.hpp>
parserTypes::expr::Expr::~Expr() {}
parserTypes::expr::BaseExpr& parserTypes::expr::Expr::optimize() {
  return *this;
}
void parserTypes::expr::Expr::eval(parserCore& ctx, int dest) {
  int offs = ctx.Asm->stack_offset;
  std::vector<int> stackOffsets;

  if (this->parts.size() == 1) {
    this->parts[0].eval(ctx, dest);
  } else {
    // write all
    for (auto elem : this->parts) {
      elem.eval(ctx, dest);
      stackOffsets.emplace_back(ctx.Asm->push(dest));
    }

    ctx.Asm->writeRegister(0, dest);
    for (auto i : stackOffsets) {
      ctx.Asm->pop(i, 14);
      ctx.stream << "add r" << dest << ", r" << dest << ", r14" << std::endl;
    }
  }

  ctx.Asm->stack_offset = offs;
}