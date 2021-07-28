#pragma once
#include <string>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class Return : public BaseStmt {
 public:
  ~Return();
  void exec(parserCore& ctx) override;
};
}  // namespace stmt

}  // namespace parserTypes
