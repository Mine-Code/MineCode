#include <expr/term.hpp>
parserTypes::expr::term::~term() {}
parserTypes::expr::BaseExpr& parserTypes::expr::term::optimize() {
  return *this;
}
void parserTypes::expr::term::eval(parserCore& ctx, int dest) {}