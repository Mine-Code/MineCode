#include <parserTypes.h>

using namespace parserTypes;

term &optimize(term &val) {
  int immutable_mul = 0;
  int immutable_div = 0;
  term newTerm;
  // process of mul
  for (auto part : val.parts) {
    if (part.value.isSingle() && part.value.parts[0].type == power::IMM) {
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
    power powerElem;
    powerElem.type = power::IMM;
    powerElem.imm = immutable_mul;

    expo expoElem;
    expoElem.parts.emplace_back(powerElem);

    expo_wrap wrapElem;
    wrapElem.type = expo_wrap::MUL;
    wrapElem.value = expoElem;

    val.parts.emplace_back(wrapElem);
  }
  {  // add div
    power powerElem;
    powerElem.type = power::IMM;
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

expr &optimize(expr &val) {
  int immutable = 0;
  expr newExpr;
  for (auto _part : val.parts) {
    auto part = optimize(_part);
    if (part.isSingle() && part.parts[0].value.isSingle() &&
        part.parts[0].value.parts[0].type == power::IMM) {
      immutable += part.parts[0].value.parts[0].imm;
    } else {
      newExpr.parts.emplace_back(part);
    }
  }
  if (immutable != 0) {
    power value;
    value.type = power::IMM;
    value.imm = immutable;

    expo exponent;
    exponent.parts.emplace_back(value);

    expo_wrap wrap;
    wrap.type = expo_wrap::MUL;
    wrap.value = exponent;

    term Term;
    Term.parts.emplace_back(wrap);

    val.parts.emplace_back(Term);
  }

  val.parts = newExpr.parts;  // copy new expr (stack safe)
  return val;
}
