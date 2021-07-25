#pragma once

#include <parserTypes.h>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

class Variable : public BasePrimary {
 public:
  std::wstring name;

  ~Variable() override;
  void eval(parserCore&) override;
};
}  // namespace primary
}  // namespace parserTypes