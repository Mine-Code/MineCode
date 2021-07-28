#pragma once
#include <string>

#include "stmt.hpp"
namespace parserTypes {
namespace stmt {
class Mcl : public BaseStmt {
 public:
  Mcl(std::string name);
  Mcl(std::wstring name);
  ~Mcl();

  std::string name;
  void exec(parserCore& ctx) override;
};
}  // namespace stmt

}  // namespace parserTypes
