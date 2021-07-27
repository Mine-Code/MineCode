#pragma once

#include "for.hpp"
#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class ForRange : public BaseFor {
  ~ForRange();

  int start;
  int end;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
