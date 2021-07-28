#include <asm.h>
#include <cond/eval.h>
#include <mcl.h>
#include <parserCore.h>

#include <stmt/mcl.hpp>
parserTypes::stmt::Mcl::~Mcl() {}
void parserTypes::stmt::Mcl::exec(parserCore& ctx) {
  (*ctx.wraper) << this->name;
}