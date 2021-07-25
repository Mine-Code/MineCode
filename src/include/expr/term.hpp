#pragma once

#include <vector>

namespace parserTypes {
struct term {
  std::vector<struct expo_wrap> parts;
  bool isSingle();
};
}  // namespace parserTypes