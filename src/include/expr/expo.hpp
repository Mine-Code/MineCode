#pragma once

#include <primary/primary.hpp>
#include <vector>

#include "base.hpp"
namespace parserTypes {
namespace expr {
class expo : public BaseExpr {
 public:
  std::vector<primary::BasePrimary*> parts;

  ~expo();

  void eval(parserCore& ctx, int) override;
  BaseExpr& optimize() override;
};
}  // namespace expr
}  // namespace parserTypes