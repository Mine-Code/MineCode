#pragma once

#include <parserCore.h>

#include <string>
#include <vector>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class FuncDef {
 public:
  ~FuncDef();

  std::wstring name;
  std::vector<parserCore::Arg> args;
  std::vector<BaseStmt*> stmts;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
