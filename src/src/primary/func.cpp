#include <parserCore.h>
#include <stmtProcessor.h>

#include <primary/func.hpp>
parserTypes::primary::FuncCall::~FuncCall() {}
void parserTypes::primary::FuncCall::eval(parserCore& ctx, int dest) {
  stmtProcessor::executeFunction(&ctx, *this->func);
  ctx.Asm->moveResister(3, dest);
}