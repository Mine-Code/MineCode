#pragma once

#include <memory>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

class Pointer : public BasePrimary {
 public:
  Expr& pointer;

  Pointer(Expr& pointer);

  ~Pointer() override;
  void eval(parserCore& ctx, int dest) override;
};
}  // namespace primary
}  // namespace parserTypes