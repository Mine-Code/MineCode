#include <parserTypes.h>

parserTypes::ExecFunc::ExecFunc()
    : args(std::vector<Expr>()),
      funcId(L"0x00000000"),
      funcAddr(0),
      type(ADDRESS) {}

bool parserTypes::Expr::isSingle() { return parts.size() == 1; }
bool parserTypes::expo::isSingle() { return parts.size() == 1; }
bool parserTypes::term::isSingle() { return parts.size() == 1; }

parserTypes::ptr::ptr() : base(nullptr) {}
parserTypes::ptr::ptr(Expr &base) : base(std::make_unique<struct Expr>(base)) {}
parserTypes::ptr::ptr(Expr *base)
    : base(std::make_unique<struct Expr>(*base)) {}
parserTypes::Expr parserTypes::ptr::getBase() { return *base; }