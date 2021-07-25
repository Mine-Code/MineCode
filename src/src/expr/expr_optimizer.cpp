#include <parserTypes.h>

using namespace parserTypes;

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
