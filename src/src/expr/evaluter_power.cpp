#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <stmtProcessor.h>
#include <syntaxError.h>
#include <util.h>

using namespace parserTypes;

void eval::Power(parserCore *that, power obj, int dest) {
  int offs = that->Asm->stack_offset;
  switch (obj.type) {
    case power::EXPR:
      Expr(that, obj.expr, dest);
      break;
    case power::FLT:
      synErr::processError(that, L"Float isn't supported...", __FILE__,
                           __func__, __LINE__);
      break;
    case power::FUNCCALL:
      stmtProcessor::executeFunction(that, *obj.func);
      that->Asm->moveResister(3, dest);
      break;
    case power::IMM:
      that->Asm->writeRegister(obj.imm, dest);
      break;
    case power::PTR:
      Ptr(that, obj.Pointer, dest);
      break;
    case power::VAR:
      Var(that, obj.var, dest);
      break;
    default:
      synErr::processError(
          that, L"unknown type error [" + std::to_wstring(obj.type) + L"]",
          __FILE__, __func__, __LINE__);
      break;
  }
  that->Asm->stack_offset = offs;
}