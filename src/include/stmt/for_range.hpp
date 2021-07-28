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
  void exec(parserCore& ctx) override;
};
}  // namespace stmt

}  // namespace parserTypes
