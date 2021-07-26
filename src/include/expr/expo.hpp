#pragma once

#include <primary/primary.hpp>
#include <vector>
namespace parserTypes {
struct expo {
  std::vector<primary::BasePrimary*> parts;
  bool isSingle();
};
}  // namespace parserTypes