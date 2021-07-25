#pragma once

#include "primary.hpp"

namespace parserTypes {
namespace primary {

class Immutable : BasePrimary {
 public:
  Immutable(int val);
  int value;

  void eval(parserCore&) override;
};
}  // namespace primary
}  // namespace parserTypes