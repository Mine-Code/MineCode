#pragma once

#include <expr/expr_t.hpp>
#include <string>

#include "for.hpp"
#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class ForIter : BaseFor {
 public:
  ~ForIter();

  std::wstring target;
  expr iter;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
