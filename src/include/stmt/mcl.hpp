#pragma once
#include <string>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class Mcl {
 public:
  ~Mcl();

  std::string name;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
