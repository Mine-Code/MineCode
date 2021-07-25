#include <asm.h>
#include <parserCore.h>
#include <syntaxError.h>
#include <util.h>

#include <value/ident.hpp>
parserTypes::value::Ident::~Ident() {}
void parserTypes::value::Ident::eval(parserCore& ctx, int dest) {
  auto var = ctx.variables[util::wstr2str(this->ident)];
  if (var.type == varType::INT) {
    ctx.Asm->pop(var.offset, dest);
  } else {
    synErr::processError(&ctx, L"float is not supported.", __FILE__, __func__,
                         __LINE__);
  }
}