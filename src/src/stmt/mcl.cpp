#include <asm.h>
#include <cond/eval.h>
#include <mcl.h>
#include <parserCore.h>
#include <util.h>

#include <stmt/mcl.hpp>
parserTypes::stmt::Mcl::Mcl(std::wstring name) : Mcl(util::wstr2str(name)) {}
parserTypes::stmt::Mcl::Mcl(std::string name) : name(name) {}
parserTypes::stmt::Mcl::~Mcl() {}
void parserTypes::stmt::Mcl::exec(parserCore& ctx) {
  (*ctx.wraper) << this->name;
}