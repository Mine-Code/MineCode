#pragma once

#include <parserTypes.h>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

class Variable : BasePrimary {
 public:
  std::wstring name;
  void eval(parserCore&) override;
};
}  // namespace primary
}  // namespace parserTypes