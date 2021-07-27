#pragma once
#include <expr/expr_t.hpp>
#include <primary/primary.hpp>
#include <string>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class Assign {
  ~Assign();

  primary::BasePrimary& dest;
  std::string op;
  expr val;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
