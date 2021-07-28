#pragma once

#include <expr/expr_t.hpp>
#include <string>

#include "for.hpp"
#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class ForIter : public BaseFor {
 public:
  virtual ~ForIter();

  std::wstring target;
  ::parserTypes::Expr iter;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
