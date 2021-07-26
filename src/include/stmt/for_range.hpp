#pragma once

#include "stmt.hpp"

namespace parserTypes {
namespace stmt {
class ForRange {
  ~ForRange();

  int start;
  int end;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
