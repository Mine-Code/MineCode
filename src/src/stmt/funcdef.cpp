#include <parserCore.h>

#include <stmt/funcdef.hpp>
parserTypes::stmt::FuncDef::~FuncDef() {}
void parserTypes::stmt::FuncDef::exec(parserCore& ctx) {
  ctx.stream << "# Inner Function\n";
  while (ctx.iter.hasData()) {
    if (ctx.iter.peek() == L"}") break;
    ctx.stmt();
  }
  ctx.stream << "# Outer Function\n";
}
