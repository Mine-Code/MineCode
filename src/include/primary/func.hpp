#pragma once

#include <parserTypes.h>

#include "primary.hpp"

namespace parserTypes {
namespace primary {
struct FuncCall : BasePrimary {
  ExecFunc func;
};
}  // namespace primary
}  // namespace parserTypes