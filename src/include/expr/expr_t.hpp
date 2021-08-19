#pragma once

#include <vector>

#include "base.hpp"
#include "term.hpp"

namespace parserTypes {
namespace expr {
class Expr : public BaseExpr {
 public:
  std::vector<term> parts;

  virtual ~Expr();

  void eval(parserCore& ctx, int) override;
  BaseExpr& optimize() override;

  template <typename T>
  bool is() {
    return dynamic_cast<T*>(this);
  }
};
}  // namespace expr
}  // namespace parserTypes