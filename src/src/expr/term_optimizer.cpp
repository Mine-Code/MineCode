#include <parserTypes.h>

using namespace parserTypes;

term &optimize(term &val) {
  int immutable_mul = 0;
  int immutable_div = 0;
  term newTerm;
  // process of mul
  for (auto part : val.parts) {
    if (part.value.isSingle() && part.value.parts[0].type == primary::IMM) {
      auto imm = part.value.parts[0].imm;
      if (part.type == expo_wrap::MUL)
        immutable_mul *= imm;
      else if (part.type == expo_wrap::DIV)
        immutable_div *= imm;
      else if (part.type == expo_wrap::MOD)
        newTerm.parts.emplace_back(part);
    } else {
      newTerm.parts.emplace_back(part);
    }
  }

  val.parts = newTerm.parts;

  // add mul/div element
  {  // add mul
    primary powerElem;
    powerElem.type = primary::IMM;
    powerElem.imm = immutable_mul;

    expo expoElem;
    expoElem.parts.emplace_back(powerElem);

    expo_wrap wrapElem;
    wrapElem.type = expo_wrap::MUL;
    wrapElem.value = expoElem;

    val.parts.emplace_back(wrapElem);
  }
  {  // add div
    primary powerElem;
    powerElem.type = primary::IMM;
    powerElem.imm = immutable_div;

    expo expoElem;
    expoElem.parts.emplace_back(powerElem);

    expo_wrap wrapElem;
    wrapElem.type = expo_wrap::DIV;
    wrapElem.value = expoElem;

    val.parts.emplace_back(wrapElem);
  }

  return val;
}