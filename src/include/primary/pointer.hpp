#pragma once

#include <expr/ptr.hpp>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

struct Pointer : BasePrimary {
  ptr pointer;
};
}  // namespace primary
}  // namespace parserTypes