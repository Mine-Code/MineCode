#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <stmtProcessor.h>
#include <syntaxError.h>
#include <util.h>

using namespace parserTypes;

void eval::Power(parserCore *that, primary obj, int dest) {
  int offs = that->Asm->stack_offset;
  switch (obj.type) {
    case primary::EXPR:
      Expr(that, obj.expr, dest);
      break;
    case primary::FLT:
      synErr::processError(that, L"Float isn't supported...", __FILE__,
                           __func__, __LINE__);
      break;
    case primary::FUNCCALL:
      stmtProcessor::executeFunction(that, *obj.func);
      that->Asm->moveResister(3, dest);
      break;
    case primary::IMM:
      that->Asm->writeRegister(obj.imm, dest);
      break;
    case primary::PTR:
      Ptr(that, obj.Pointer, dest);
      break;
    case primary::VAR:
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