#include <parserCore.h>
#include <parserTypes.h>

#include <expr/inverted.hpp>

using parserTypes::expr::BaseExpr;
using parserTypes::expr::Inverted;
Inverted::~Inverted() {}

BaseExpr& Inverted::optimize() {
  auto child = expr_.as<Inverted>();
  if (child) {
    return child->expr_.optimize();
  }

  return *this;
}
void Inverted::eval(parserCore& ctx, int dest) {}