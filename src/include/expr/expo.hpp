#pragma once

#include <primary/primary.hpp>
#include <vector>

#include "base.hpp"
namespace parserTypes {
class expo : public BaseExpr {
 public:
  std::vector<primary::BasePrimary*> parts;

  void eval(parserCore& ctx) override;
  BaseExpr& optimize() override;
};
}  // namespace parserTypes