#pragma once

class parserCore;

namespace parserTypes {
namespace primary {

struct BasePrimary {
  BasePrimary();
  virtual ~BasePrimary();
  virtual void eval(parserCore&);
};
}  // namespace primary
}  // namespace parserTypes