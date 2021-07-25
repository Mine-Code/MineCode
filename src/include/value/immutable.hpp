#pragma once
#include <parserCore.h>

#include "value.hpp"
namespace parserTypes {
namespace value {
struct Immutable : BaseValue {
  uint32_t imm;

  ~Immutable();
  void eval(parserCore& ctx, int dest) override;
};
}  // namespace value
}  // namespace parserTypes