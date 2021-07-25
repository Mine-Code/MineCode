
#pragma once

#include <inttypes.h>

#include <types/expr.hpp>
#include <vector>

#include "and.hpp"

namespace parserTypes {
struct cond {
  std::vector<struct condAnd> conds;
};
}  // namespace parserTypes