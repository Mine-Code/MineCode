#include <eval.h>

#include <primary/inner.hpp>
void parserTypes::primary::Inner::eval(parserCore& ctx, int dest) {
  ::eval::Expr(&ctx, this->expr, dest);
}