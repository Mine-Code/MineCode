#pragma once

#include <expr/ptr.hpp>
#include <string>
namespace parserTypes {
namespace value {
class BaseValue {
  BaseValue();
  virtual ~BaseValue();

  virtual void eval(parserCore& ctx, int dest) = 0;
};
}  // namespace value
}  // namespace parserTypes