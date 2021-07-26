#pragma once

class parserCore;

namespace parserTypes {
namespace stmt {
class BaseStmt {
  BaseStmt();
  ~BaseStmt();

  virtual void exec(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
