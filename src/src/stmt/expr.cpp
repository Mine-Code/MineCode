#include <asm.h>
#include <cond/eval.h>
#include <parserCore.h>

#include <stmt/expr.hpp>
parserTypes::stmt::Expr::~Expr() {}
void parserTypes::stmt::Expr::exec(parserCore& ctx) { this->exec(ctx); }