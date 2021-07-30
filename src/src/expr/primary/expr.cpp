#include <eval.h>

#include <primary/inner.hpp>
void parserTypes::primary::Inner::eval(parserCore& ctx, int dest) {
  this->expr.eval(ctx, dest);
}