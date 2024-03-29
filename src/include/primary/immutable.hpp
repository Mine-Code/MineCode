#pragma once

#include "primary.hpp"

namespace parserTypes {
namespace primary {

class Immutable : public BasePrimary {
 public:
  Immutable(int val);
  int value;

  void eval(parserCore& ctx, int dest) override;
};
}  // namespace primary
}  // namespace parserTypes