#pragma once

#include <vector>

#include "primary.hpp"
namespace parserTypes {
struct expo {
  std::vector<struct primary> parts;
  bool isSingle();
};
}  // namespace parserTypes