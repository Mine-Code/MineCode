#pragma once

#include <vector>

namespace parserTypes {
struct expo {
  std::vector<struct power> parts;
  bool isSingle();
};
}  // namespace parserTypes