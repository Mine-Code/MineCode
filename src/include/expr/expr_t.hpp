#pragma once

#include <vector>

#include "base.hpp"
#include "term.hpp"

namespace parserTypes {
class Expr : public BaseExpr {
 public:
  std::vector<term> parts;

  void eval(parserCore& ctx) override;
  BaseExpr& optimize() override;
};
}  // namespace parserTypes