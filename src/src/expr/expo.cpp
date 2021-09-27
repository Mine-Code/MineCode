#include <parserCore.h>
#include <parserTypes.h>

#include <expr/expo.hpp>

using namespace parserTypes;
parserTypes::expr::expo::~expo() {}
parserTypes::expr::BaseExpr& parserTypes::expr::expo::optimize() {
  return *this;
}
void parserTypes::expr::expo::eval(parserCore& ctx, int dest) {
  int offs = ctx.Asm->stack_offset;
  std::vector<int> stackOffsets;

  if (this->parts.size() == 1) {
    this->parts[0]->eval(ctx, dest);
  } else {
    // write all
    for (auto elem : this->parts) {
      elem->eval(ctx, dest);
      stackOffsets.emplace_back(ctx.Asm->push(dest));
    }
    // TODO: power all stackOffsets
  }
  ctx.Asm->stack_offset = offs;
}