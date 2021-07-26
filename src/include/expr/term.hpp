#pragma once

#include <vector>

#include "expo_wrap.hpp"
namespace parserTypes {
struct term {
  std::vector<struct expo_wrap> parts;
  bool isSingle();
};
}  // namespace parserTypes