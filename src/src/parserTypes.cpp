#include <parserTypes.h>

parserTypes::ExecFunc::ExecFunc()
    : args(std::vector<expr>()),
      funcId(L"0x00000000"),
      funcAddr(0),
      type(ADDRESS) {}

parserTypes::power::power() : type(power::IMM), imm(0) {}

bool parserTypes::expr::isSingle() { return parts.size() == 1; }
bool parserTypes::expo::isSingle() { return parts.size() == 1; }
bool parserTypes::term::isSingle() { return parts.size() == 1; }

parserTypes::ptr::ptr() : base(nullptr) {}
parserTypes::ptr::ptr(expr &base) : base(std::make_unique<struct expr>(base)) {}
parserTypes::ptr::ptr(expr *base)
    : base(std::make_unique<struct expr>(*base)) {}
parserTypes::expr parserTypes::ptr::getBase() { return *base; }