#pragma once

#include <vector>

#include "base.hpp"
#include "expo.hpp"

namespace parserTypes {
namespace expr {
struct expo_wrap {
  enum Type { MUL, DIV, MOD };

  Type type;
  expo value;
};
class term : public BaseExpr {
 public:
  std::vector<struct expo_wrap> parts;

  ~term();

  void eval(parserCore& ctx, int) override;
  BaseExpr& optimize() override;
};

}  // namespace expr
}  // namespace parserTypes