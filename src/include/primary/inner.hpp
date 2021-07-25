#pragma once

#include <expr/expr.hpp>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

struct Inner : BasePrimary {
  expr inner;
};
}  // namespace primary
}  // namespace parserTypes