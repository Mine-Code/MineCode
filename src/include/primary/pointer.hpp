#pragma once

#include <expr/ptr.hpp>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

class Pointer : public BasePrimary {
 public:
  ptr pointer;

  ~Pointer() override;
  void eval(parserCore& ctx, int dest) override;
};
}  // namespace primary
}  // namespace parserTypes