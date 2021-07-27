#pragma once

#include <expr/expr_t.hpp>
#include <string>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class ForIter {
  ~ForIter();

  std::wstring target;
  expr iter;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
