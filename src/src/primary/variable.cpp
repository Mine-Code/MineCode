#include <eval.h>

#include <primary/variable.hpp>
parserTypes::primary::Variable::~Variable() {}
void parserTypes::primary::Variable::eval(parserCore& ctx, int dest) {
  ::eval::Var(&ctx, this->name, dest);
}