#pragma once

#include <vector>
class parserCore;
namespace parserTypes {
struct BaseExpr {
  virtual ~BaseExpr();
  virtual void eval(parserCore& ctx) = 0;
  [[nodiscard]] virtual BaseExpr& optimize() = 0;
};
}  // namespace parserTypes