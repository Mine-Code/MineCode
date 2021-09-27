#include <asm.h>
#include <eval.h>
#include <parserCore.h>

#include <stmt/expr.hpp>
parserTypes::stmt::Expr::~Expr() {}
void parserTypes::stmt::Expr::exec(parserCore& ctx) { this->val.eval(ctx, 14); }