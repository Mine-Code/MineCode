#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <stmtProcessor.h>
#include <syntaxError.h>
#include <util.h>

using namespace parserTypes;

void Ptr_AddrBase(parserCore *that, ptr obj, int dest) {
  eval::Expr(that, obj.getBase(), dest);
}

void eval::Expr(parserCore *that, expr obj, int dest) {
  int offs = that->Asm->stack_offset;
  std::vector<int> stackOffsets;

  if (obj.isSingle()) {
    Term(that, obj.parts[0], dest);
  } else {
    // write all
    for (auto elem : obj.parts) {
      Term(that, elem, dest);
      stackOffsets.emplace_back(that->Asm->push(dest));
    }

    that->Asm->writeRegister(0, dest);
    for (auto i : stackOffsets) {
      that->Asm->pop(i, 14);
      that->stream << "add r" << dest << ", r" << dest << ", r14" << std::endl;
    }
  }

  that->Asm->stack_offset = offs;
}
void eval::Expo(parserCore *that, expo obj, int dest) {
  int offs = that->Asm->stack_offset;
  std::vector<int> stackOffsets;

  if (obj.isSingle()) {
    Power(that, obj.parts[0], dest);
  } else {
    // write all
    for (auto elem : obj.parts) {
      Power(that, elem, dest);
      stackOffsets.emplace_back(that->Asm->push(dest));
    }
    // TODO: power all stackOffsets
  }
  that->Asm->stack_offset = offs;
}
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
void eval::Ptr(parserCore *that, ptr obj, int dest) {
  int offs = that->Asm->stack_offset;
  Ptr_AddrBase(that, obj, dest);
  that->Asm->peek(0, dest, dest);
  that->Asm->stack_offset = offs;
}
void eval::Ptr_Addr(parserCore *that, ptr obj, int dest) {
  Ptr_AddrBase(that, obj, dest);
}
