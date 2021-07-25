#pragma once
#include <parserCore.h>

#include "value.hpp"
namespace parserTypes {
namespace value {
class Ident : public BaseValue {
 public:
  std::wstring ident;

  ~Ident();
  void eval(parserCore& ctx, int dest) override;
};
}  // namespace value
}  // namespace parserTypes