#include <expr/expr_t.hpp>
parserTypes::expr::Expr::~Expr() {}
parserTypes::expr::BaseExpr& parserTypes::expr::Expr::optimize() {
  return *this;
}
void parserTypes::expr::Expr::eval(parserCore& ctx, int dest) {}