#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>

#include <primary/pointer.hpp>
using namespace parserTypes;

void Ptr_AddrBase(parserCore *that, primary::Pointer &obj, int dest) {
  eval::Expr(that, obj.getBase(), dest);
}

void eval::Ptr(parserCore *that, primary::Pointer &obj, int dest) {
  int offs = that->Asm->stack_offset;
  Ptr_AddrBase(that, obj, dest);
  that->Asm->peek(0, dest, dest);
  that->Asm->stack_offset = offs;
}
void eval::Ptr_Addr(parserCore *that, primary::Pointer &obj, int dest) {
  Ptr_AddrBase(that, obj, dest);
}
