#pragma once

#include <expr/expr.hpp>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

struct Inner : BasePrimary {
  expr expr;
};
}  // namespace primary
}  // namespace parserTypes