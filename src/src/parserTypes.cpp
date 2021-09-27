#include <parserTypes.h>

parserTypes::ExecFunc::ExecFunc()
    : args(std::vector<expr::Expr>()),
      funcId(L"0x00000000"),
      funcAddr(0),
      type(ADDRESS) {}
