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

  template <typename T>
  bool is() {
    return this->as<T>() != nullptr;
  }

  template <typename T>
  T* as() {
    return dynamic_cast<T*>(this);
  }
};
}  // namespace expr
}  // namespace parserTypes