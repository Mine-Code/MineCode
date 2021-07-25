#pragma once

#include <vector>

#include "expo.hpp"
namespace parserTypes {
struct expo_wrap {
  enum Type { MUL, DIV, MOD };

  Type type;
  struct expo value;
};
}  // namespace parserTypes