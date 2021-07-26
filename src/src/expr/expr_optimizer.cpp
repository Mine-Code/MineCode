#include <parserTypes.h>

#include <primary/immutable.hpp>
using namespace parserTypes;
term &optimize(term &);
expr &optimize(expr &val) {
  int immutable = 0;
  expr newExpr;
  for (auto _part : val.parts) {
    auto part = optimize(_part);
    if (part.isSingle() && part.parts[0].value.isSingle() &&
        typeid(part.parts[0].value.parts[0]) == typeid(primary::Immutable)) {
      immutable +=
          dynamic_cast<primary::Immutable *>(part.parts[0].value.parts[0])
              ->value;
    } else {
      newExpr.parts.emplace_back(part);
    }
  }
  if (immutable != 0) {
    auto value = new primary::Immutable(immutable);

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
