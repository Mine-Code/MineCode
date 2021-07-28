#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <syntaxError.h>
#include <util.h>

using namespace parserTypes;

void eval::Power(parserCore *that, primary::BasePrimary &obj, int dest) {
  int offs = that->Asm->stack_offset;
  obj.eval(*that, dest);
  that->Asm->stack_offset = offs;
}