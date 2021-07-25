#pragma once

#include <parserTypes.h>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

struct Variable : BasePrimary {
  std::wstring variable_name;
};
}  // namespace primary
}  // namespace parserTypes