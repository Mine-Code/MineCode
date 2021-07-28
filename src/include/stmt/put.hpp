#pragma once
#include <expr/expr_t.hpp>
#include <string>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class Put : public BaseStmt {
 public:
  ~Put();

  std::string dest;
  std::wstring val;
  void exec(parserCore& ctx) override;
};
}  // namespace stmt

}  // namespace parserTypes
