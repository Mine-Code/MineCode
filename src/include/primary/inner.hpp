#pragma once

#include <expr/expr_t.hpp>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

class Inner : public BasePrimary {
 public:
  // TODO: restore name inner -> expr
  expr inner;

  void eval(parserCore&) override;
};
}  // namespace primary
}  // namespace parserTypes