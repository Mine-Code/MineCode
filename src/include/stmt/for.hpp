#pragma once

#include <expr/expr_t.hpp>
#include <string>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class BaseFor {
  std::vector<BaseStmt*> stmts;
};
}  // namespace stmt

}  // namespace parserTypes
