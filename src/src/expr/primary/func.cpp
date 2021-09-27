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
    arg.eval(ctx, n++);
  }

  // call function
  if (util::in(ctx.function_list, this->func->funcId)) {
    ctx.Asm->Jump(this->func->funcId);
    return;
  }

  // address based function call
  auto func_id = util::wstr2str(this->func->funcId);
  if (this->func->type == ExecFunc::ADDRESS) {  // address based
    ctx.Asm->writeRegister(this->func->funcAddr, 15);
  } else if (this->func->type == ExecFunc::Name) {  // name based
    if (util::in(ctx.variables, func_id)) {
      ctx.Asm->pop(ctx.variables[func_id].offset, 15);
    } else if (util::in(ctx.functions, func_id)) {
      if (!util::in(ctx.functions, func_id)) {
        processError(&ctx, this->func->funcId + L" is not found", __FILE__,
                     __func__, __LINE__);
      }
      ctx.Asm->writeRegister(ctx.functions[func_id].addr, 15);
    } else {
      processError(&ctx, L"Function not found: " + this->func->funcId, __FILE__,
                   __func__, __LINE__);
    }
  }
  ctx.stream << "mtctr r15\n"
                "bctrl\n";
  ctx.Asm->moveResister(3, dest);
}