#pragma once

#include <string>
#include <vector>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class FuncDef {
  ~FuncDef();

  std::string name;
  std::vector<BaseStmt*> stmts;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
