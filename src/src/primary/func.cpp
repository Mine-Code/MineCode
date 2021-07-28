#include <asm.h>
#include <eval.h>
#include <parserCore.h>
#include <syntaxError.h>
#include <util.h>

#include <primary/func.hpp>

using namespace synErr;
parserTypes::primary::FuncCall::~FuncCall() {}
void parserTypes::primary::FuncCall::eval(parserCore& ctx, int dest) {
  // load arguments
  int n = 3;

  for (auto arg : this->func->args) {
    eval::Expr(&ctx, arg, n++);
  }
  // load address
  if (this->func->type == ExecFunc::ADDRESS) {
    // address based
    ctx.Asm->writeRegister(this->func->funcAddr, 15);
  } else if (this->func->type == ExecFunc::Name) {
    // name based
    if (util::in(ctx.variables, util::wstr2str(this->func->funcId))) {
      ctx.Asm->pop(ctx.variables[util::wstr2str(this->func->funcId)].offset,
                   15);
    } else if (util::in(ctx.functions, util::wstr2str(this->func->funcId))) {
      if (!util::in(ctx.variables, util::wstr2str(this->func->funcId))) {
        processError(&ctx, this->func->funcId + L" is not found", __FILE__,
                     __func__, __LINE__);
      }
      ctx.Asm->writeRegister(
          ctx.functions[util::wstr2str(this->func->funcId)].addr, 15);
    } else {
      processError(&ctx, L"Function not found: " + this->func->funcId, __FILE__,
                   __func__, __LINE__);
    }
  }
  ctx.stream << "mtctr r15\n"
                "bctrl\n";
  ctx.Asm->moveResister(3, dest);
}