#pragma once
#include <expr/expr_t.hpp>
#include <primary/primary.hpp>
#include <string>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class Assign : public BaseStmt {
 public:
  Assign(primary::BasePrimary& dest);
  ~Assign();

  primary::BasePrimary& dest;
  std::wstring op;
  ::parserTypes::Expr val;
  void exec(parserCore& ctx) override;
};
}  // namespace stmt

}  // namespace parserTypes
