#pragma once

#include <cond/cond.hpp>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class If : public BaseStmt {
 public:
  ~If();

  cond conditional;
  std::vector<BaseStmt*> stmts;
  void exec(parserCore& ctx) override;
};
}  // namespace stmt

}  // namespace parserTypes
