#include <expr/expr_t.hpp>
parserTypes::Expr::~Expr() {}
parserTypes::BaseExpr& parserTypes::Expr::optimize() { return *this; }
void parserTypes::Expr::eval(parserCore& ctx, int dest) {}