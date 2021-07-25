#pragma once

#include <string>

class parserCore;
namespace parserTypes {
namespace value {
class BaseValue {
 public:
  BaseValue();
  virtual ~BaseValue();

  virtual void eval(parserCore& ctx, int dest) = 0;
};
}  // namespace value
}  // namespace parserTypes