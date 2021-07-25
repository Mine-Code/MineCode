#pragma once
#ifndef STMT_PROC_H
#define STMT_PROC_H

#include <string>
class parserCore;
namespace parserTypes {
class ExecFunc;
class expr;
class cond;
namespace value {
class BaseValue;
}
}  // namespace parserTypes

namespace stmtProcessor {
using String = std::wstring;

void For(parserCore *that, String, String);
void Forr(parserCore *that, int, int);
void While(parserCore *that, parserTypes::cond conditional);
void If(parserCore *that, struct parserTypes::cond conditional);
void Func(parserCore *that);
void Put(parserCore *that);
void Assign(parserCore *that, parserTypes::value::BaseValue target,
            std::wstring op, struct parserTypes::expr &value);
void executeFunction(parserCore *that, parserTypes::ExecFunc);
}  // namespace stmtProcessor

#endif