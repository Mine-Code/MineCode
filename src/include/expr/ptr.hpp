#pragma once

#include <memory>

namespace parserTypes {
class Expr;
class ptr {
  std::shared_ptr<Expr> base;

 public:
  ptr(Expr &_base);
  ptr(Expr *_base);
  ptr();

  Expr getBase();
};
}  // namespace parserTypes