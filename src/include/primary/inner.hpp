#pragma once

#include <expr/expr.hpp>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

struct Inner : BasePrimary {
  // TODO: restore name inner -> expr
  expr inner;
};
}  // namespace primary
}  // namespace parserTypes