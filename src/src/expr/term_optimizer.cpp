#include <parserTypes.h>

#include <primary/immutable.hpp>

using namespace parserTypes;

expr::term &optimize(expr::term &val) {
  int immutable_mul = 0;
  int immutable_div = 0;
  expr::term newTerm;
  // process of mul
  for (auto part : val.parts) {
    if (!part.value.parts.size() == 1) {
      newTerm.parts.emplace_back(part);
    }
    if (auto imm = dynamic_cast<primary::Immutable *>(part.value.parts[0])) {
      auto val = imm->value;
      if (part.type == expr::expo_wrap::MUL)
        immutable_mul *= val;
      else if (part.type == expr::expo_wrap::DIV)
        immutable_div *= val;
      else if (part.type == expr::expo_wrap::MOD)
        newTerm.parts.emplace_back(part);
    }
  }

  val.parts = newTerm.parts;

  // add mul/div element
  {  // add mul
    auto powerElem = new primary::Immutable(immutable_mul);

    expr::expo expoElem;
    expoElem.parts.emplace_back(powerElem);

    expr::expo_wrap wrapElem;
    wrapElem.type = expr::expo_wrap::MUL;
    wrapElem.value = expoElem;

    val.parts.emplace_back(wrapElem);
  }
  {  // add div
    auto powerElem = new primary::Immutable(immutable_div);

    expr::expo expoElem;
    expoElem.parts.emplace_back(powerElem);

    expr::expo_wrap wrapElem;
    wrapElem.type = expr::expo_wrap::DIV;
    wrapElem.value = expoElem;

    val.parts.emplace_back(wrapElem);
  }

  return val;
}