#include <asm.h>
#include <parserCore.h>

#include <primary/immutable.hpp>
parserTypes::primary::Immutable::Immutable(int val) : value(val) {}
void parserTypes::primary::Immutable::eval(parserCore& ctx, int dest) {
  ctx.Asm->writeRegister(this->value, dest);
}