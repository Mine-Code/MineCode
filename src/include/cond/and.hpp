
#pragma once

#include <inttypes.h>

#include <expr/expr.hpp>
#include <vector>

#include "primary.hpp"
namespace parserTypes {
struct condAnd {
  std::vector<struct condChild> conds;
};
}  // namespace parserTypes