
#pragma once

#include <inttypes.h>

#include <vector>

#include "and.hpp"

namespace parserTypes {
struct Cond {
  std::vector<struct condAnd> conds;
};
}  // namespace parserTypes