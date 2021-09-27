#pragma once
#include <expr/expr_t.hpp>
#include <primary/primary.hpp>
#include <string>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class Expr : public BaseStmt {
 public:
  ~Expr();

  ::parserTypes::expr::Expr val;
  void exec(parserCore& ctx) override;
};
}  // namespace stmt

}  // namespace parserTypes
