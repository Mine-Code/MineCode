#include <parserCore.h>
#include <parserTypes.h>

#include <expr/inverted.hpp>

using parserTypes::expr::BaseExpr;
using parserTypes::expr::Inverted;
Inverted::~Inverted() {}

BaseExpr& Inverted::optimize() {}
void Inverted::eval(parserCore& ctx, int dest) {}