#pragma once

#include <cond/cond.hpp>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class If {
 public:
  virtual ~If();

  cond conditional;
  std::vector<BaseStmt*> stmts;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
