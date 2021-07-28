#pragma once

#include <vector>

#include "term.hpp"

namespace parserTypes {
struct Expr {
  std::vector<struct term> parts;
  bool isSingle();
};
}  // namespace parserTypes