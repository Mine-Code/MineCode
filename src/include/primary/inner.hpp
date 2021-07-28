#pragma once

#include <expr/expr_t.hpp>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

class Inner : public BasePrimary {
 public:
  Expr expr;

  void eval(parserCore& ctx, int dest) override;
};
}  // namespace primary
}  // namespace parserTypes