#include <parserTypes.h>

#include <primary/immutable.hpp>
using namespace parserTypes;
expr::term &optimize(expr::term &);
expr::Expr &optimize(expr::Expr &val) {
  int immutable = 0;
  expr::Expr newExpr;
  for (auto _part : val.parts) {
    auto part = optimize(_part);
    if (part.parts.size() == 1 && part.parts[0].value.parts.size() == 1 &&
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

    expr::expo exponent;
    exponent.parts.emplace_back(value);

    expr::expo_wrap wrap;
    wrap.type = expr::expo_wrap::MUL;
    wrap.value = exponent;

    expr::term Term;
    Term.parts.emplace_back(wrap);

    val.parts.emplace_back(Term);
  }

  val.parts = newExpr.parts;  // copy new expr (stack safe)
  return val;
}
