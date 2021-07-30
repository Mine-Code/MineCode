#include <expr/expo.hpp>
parserTypes::expr::expo::~expo() {}
parserTypes::expr::BaseExpr& parserTypes::expr::expo::optimize() {
  return *this;
}
void parserTypes::expr::expo::eval(parserCore& ctx, int dest) {}