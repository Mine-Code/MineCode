#pragma once

#include <string>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

struct Variable : BasePrimary {
  std::wstring name;
};
}  // namespace primary
}  // namespace parserTypes