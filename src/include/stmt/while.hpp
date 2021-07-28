#pragma once

#include <cond/cond.hpp>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class While : public BaseStmt {
 public:
  ~While();

  cond conditional;
  std::vector<BaseStmt*> stmts;
  void exec(parserCore& ctx) override;
};
}  // namespace stmt

}  // namespace parserTypes
