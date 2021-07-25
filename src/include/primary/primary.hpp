#pragma once

class parserCore;

namespace parserTypes {
namespace primary {

struct BasePrimary {
  BasePrimary();
  void eval(parserCore&) = delete;
};
}  // namespace primary
}  // namespace parserTypes