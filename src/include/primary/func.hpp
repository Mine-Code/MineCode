#pragma once

#include <parserTypes.h>

#include "primary.hpp"

namespace parserTypes {
namespace primary {
class FuncCall : public BasePrimary {
 public:
  ExecFunc* func;
  ~FuncCall() override;

  void eval(parserCore&) override;
};
}  // namespace primary
}  // namespace parserTypes