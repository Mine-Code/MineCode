#pragma once

#include <expr/ptr.hpp>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

class Pointer : BasePrimary {
 public:
  ptr pointer;

  void eval(parserCore&) override;
};
}  // namespace primary
}  // namespace parserTypes