#pragma once

#include "for.hpp"
#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class ForRange : public BaseFor {
 public:
  ForRange(int begin, int end);
  ~ForRange();

  int start;
  int end;
  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
