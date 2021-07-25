#pragma once
#include <parserCore.h>

#include "value.hpp"
namespace parserTypes {
namespace value {
class Immutable : public BaseValue {
 public:
  uint32_t imm;

  ~Immutable();
  void eval(parserCore& ctx, int dest) override;
};
}  // namespace value
}  // namespace parserTypes