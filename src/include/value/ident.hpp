#pragma once
#include <parserCore.h>

#include "value.hpp"
namespace parserTypes {
namespace value {
struct Pointer : BaseValue {
  std::wstring ident;

  ~Pointer();
  void eval(parserCore& ctx, int dest) override;
};
}  // namespace value
}  // namespace parserTypes