#pragma once

class parserCore;

namespace parserTypes {
namespace primary {

class BasePrimary {
 public:
  BasePrimary();
  virtual ~BasePrimary();

  virtual void eval(parserCore& ctx, int dest) = 0;
};
}  // namespace primary
}  // namespace parserTypes