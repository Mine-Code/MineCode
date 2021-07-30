#include <eval.h>

#include <primary/pointer.hpp>
parserTypes::primary::Pointer::Pointer(Expr& expr)
    : BasePrimary(), pointer(expr) {}

parserTypes::primary::Pointer::~Pointer() {}
void parserTypes::primary::Pointer::eval(parserCore& ctx, int dest) {
  ::eval::Ptr(&ctx, this->pointer, dest);
}