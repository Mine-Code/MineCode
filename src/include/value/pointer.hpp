#pragma once
#include <parserCore.h>

#include <expr/ptr.hpp>

#include "value.hpp"
namespace parserTypes {
namespace value {
class Pointer : public BaseValue {
 public:
  ptr pointer;

  ~Pointer();
  void eval(parserCore& ctx, int dest) override;
};
}  // namespace value
}  // namespace parserTypes