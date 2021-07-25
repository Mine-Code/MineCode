#pragma once

#include <primary/primary.hpp>
#include <vector>
namespace parserTypes {
struct expo {
  std::vector<struct primary> parts;
  bool isSingle();
};
}  // namespace parserTypes