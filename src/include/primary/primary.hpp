#pragma once

class parserCore;

namespace parserTypes {
namespace primary {

class BasePrimary {
 public:
  BasePrimary();
  virtual ~BasePrimary();

  virtual void eval(parserCore&) = 0;
};
}  // namespace primary
}  // namespace parserTypes