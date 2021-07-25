#pragma once

#include <vector>

namespace parserTypes {
// TODO: rename this to Expr
struct expr {
  std::vector<struct term> parts;
  bool isSingle();
};
}  // namespace parserTypes