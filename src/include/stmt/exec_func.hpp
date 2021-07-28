#pragma once

#include "stmt.hpp"

namespace parserTypes {
class ExecFunc;
namespace stmt {
class ExecFunc : public BaseStmt {
  ~ExecFunc();

  ::parserTypes::ExecFunc func;
  void exec(parserCore& ctx) override;
};
}  // namespace stmt

}  // namespace parserTypes
