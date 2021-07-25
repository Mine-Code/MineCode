#pragma once

#include <vector>

namespace parserTypes {
struct expo_wrap {
  enum Type { MUL, DIV, MOD };

  Type type;
  struct expo value;
};
}  // namespace parserTypes