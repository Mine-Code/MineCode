#pragma once

#include "base.hpp"
namespace parserTypes {
namespace expr {
class Inverted : public BaseExpr {
 public:
  explicit Inverted(const BaseExpr& expr);
  ~Inverted();

  void eval(parserCore& ctx, int) override;
  BaseExpr& optimize() override;

 private:
  BaseExpr& expr_;
};
}  // namespace expr
}  // namespace parserTypes
