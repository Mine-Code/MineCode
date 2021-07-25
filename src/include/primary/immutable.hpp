#pragma once

#include "primary.hpp"

namespace parserTypes {
namespace primary {

struct Immutable : BasePrimary {
  Immutable(int val);
  int value;
};
}  // namespace primary
}  // namespace parserTypes