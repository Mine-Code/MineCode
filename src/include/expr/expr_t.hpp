#pragma once

#include <vector>

namespace parserTypes {
struct expr {
  std::vector<struct term> parts;
  bool isSingle();
};
}  // namespace parserTypes