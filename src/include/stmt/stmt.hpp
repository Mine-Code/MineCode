#pragma once

class parserCore;

namespace parserTypes {
namespace stmt {
class BaseStmt {
  BaseStmt();
  ~BaseStmt();

  virtual void eval(parserCore& ctx);
};
}  // namespace stmt

}  // namespace parserTypes
