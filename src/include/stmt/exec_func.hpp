#pragma once

#include "stmt.hpp"

namespace parserTypes {
class ExecFunc;
namespace stmt {
class ExecFunc {
  ~ExecFunc();

  ::parserTypes::ExecFunc& func;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
