#pragma once

#include <vector>
class parserCore;
namespace parserTypes {
struct BaseExpr {
  virtual void eval(parserCore& ctx) = 0;
  [[nodiscard("Not changed.")]] virtual BaseExpr& optimize() = 0;
};
}  // namespace parserTypes