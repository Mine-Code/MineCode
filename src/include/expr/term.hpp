#pragma once

#include <vector>

#include "expo.hpp"

namespace parserTypes {
struct expo_wrap {
  enum Type { MUL, DIV, MOD };

  Type type;
  struct expo value;
};
struct term {
  std::vector<struct expo_wrap> parts;
  bool isSingle();
};
}  // namespace parserTypes