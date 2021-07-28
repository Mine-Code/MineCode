#pragma once
#include <expr/expr_t.hpp>
#include <string>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class Put {
 public:
  virtual ~Put();

  std::string dest;
  std::wstring val;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
