#pragma once

#include <memory>

namespace parserTypes {
class expr;
class ptr {
  std::shared_ptr<expr> base;

 public:
  ptr(expr &_base);
  ptr(expr *_base);
  ptr();

  expr getBase();
};
}  // namespace parserTypes