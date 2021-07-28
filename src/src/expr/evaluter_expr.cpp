#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>
using namespace parserTypes;

void eval::Expr(parserCore *that, ::parserTypes::Expr obj, int dest) {
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