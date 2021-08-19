#pragma once

#include "base.hpp"
namespace parserTypes {
namespace expr {
class Inverted : public BaseExpr {
 public:
  ~Inverted();

  BaseExpr& expr;

  void eval(parserCore& ctx, int) override;
  BaseExpr& optimize() override;
};
}  // namespace expr
}  // namespace parserTypes
