#pragma once
#include <parserCore.h>

#include "value.hpp"
namespace parserTypes {
namespace value {
struct Str : BaseValue {
  std::wstring str;

  ~Str();
  void eval(parserCore& ctx, int dest) override;
};
}  // namespace value
}  // namespace parserTypes