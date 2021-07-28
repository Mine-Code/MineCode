#pragma once

#include <parserCore.h>

#include <string>
#include <vector>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class FuncDef : public BaseStmt {
 public:
  ~FuncDef();

  std::wstring name;
  std::vector<parserCore::Arg> args;
  std::vector<BaseStmt*> stmts;
  void exec(parserCore& ctx) override;
};
}  // namespace stmt

}  // namespace parserTypes
