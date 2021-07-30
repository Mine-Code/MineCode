#pragma once

#include "primary.hpp"

namespace parserTypes {
namespace primary {

class Pointer : public BasePrimary {
 public:
  std::shared_ptr<Expr> pointer;

  ~Pointer() override;
  void eval(parserCore& ctx, int dest) override;
};
}  // namespace primary
}  // namespace parserTypes