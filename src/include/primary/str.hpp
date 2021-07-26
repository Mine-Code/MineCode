#pragma once

#include <parserTypes.h>

#include "primary.hpp"

namespace parserTypes {
namespace primary {

class Str : public BasePrimary {
 public:
  std::wstring str;

  ~Str();
  void eval(parserCore& ctx, int dest) override;
};

}  // namespace primary
}  // namespace parserTypes