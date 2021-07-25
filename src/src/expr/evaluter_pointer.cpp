#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>

using namespace parserTypes;

void Ptr_AddrBase(parserCore *that, ptr obj, int dest) {
  eval::Expr(that, obj.getBase(), dest);
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
