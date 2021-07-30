#pragma once

#include <vector>
class parserCore;
namespace parserTypes {
namespace expr {
class BaseExpr {
 public:
  virtual ~BaseExpr();
  virtual void eval(parserCore& ctx, int dest) = 0;
  [[nodiscard]] virtual BaseExpr& optimize() = 0;
};
}  // namespace expr
}  // namespace parserTypes