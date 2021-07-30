#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>
using namespace parserTypes;

void eval::Expo(parserCore *that, expr::expo obj, int dest) {
  int offs = that->Asm->stack_offset;
  std::vector<int> stackOffsets;

  if (obj.parts.size() == 1) {
    Power(that, *obj.parts[0], dest);
  } else {
    // write all
    for (auto elem : obj.parts) {
      Power(that, *elem, dest);
      stackOffsets.emplace_back(that->Asm->push(dest));
    }
    // TODO: power all stackOffsets
  }
  that->Asm->stack_offset = offs;
}