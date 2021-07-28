#pragma once

class parserCore;

namespace parserTypes {
namespace stmt {
class BaseStmt {
 public:
  BaseStmt();
  virtual ~BaseStmt();

  virtual void exec(parserCore& ctx) = 0;
};
}  // namespace stmt

}  // namespace parserTypes
