#pragma once
#include <expr/expr_t.hpp>
#include <string>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class Put {
  ~Put();

  std::string dest;
  expr val;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
