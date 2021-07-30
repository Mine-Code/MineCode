#pragma once

#include <vector>

#include "base.hpp"
#include "expo.hpp"

namespace parserTypes {
struct expo_wrap {
  enum Type { MUL, DIV, MOD };

  Type type;
  expo value;
};
class term : public BaseExpr {
 public:
  std::vector<struct expo_wrap> parts;

  void eval(parserCore& ctx) override;
  BaseExpr& optimize() override;
};
}  // namespace parserTypes