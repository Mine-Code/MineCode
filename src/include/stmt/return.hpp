#pragma once
#include <string>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class Return {
  ~Return();
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
