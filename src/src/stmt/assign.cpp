#include <eval.h>
#include <parserCore.h>
#include <syntaxError.h>
#include <util.h>

#include <primary/all.hpp>
#include <stmt/assign.hpp>
parserTypes::stmt::Assign::Assign(parserTypes::primary::BasePrimary &dest)
    : dest(dest), op(L"="), val(::parserTypes::Expr()) {}
parserTypes::stmt::Assign::~Assign() {}
void parserTypes::stmt::Assign::exec(parserCore &ctx) {
  using namespace parserTypes::primary;
  using namespace synErr;
  if (Variable *ident = dynamic_cast<Variable *>(&this->dest)) {
    std::string target = util::wstr2str(ident->name);
    // check: is avail variable of target
    if (ctx.variables.count(target) == 0) {
      // check: is [op==equal and not have element]
      if (op == L"=") {
        // make variable
        varType var;

        ctx.Asm->stack_offset += 4;
        var.offset = ctx.Asm->stack_size - ctx.Asm->stack_offset + 4;

        ctx.variables[target] = var;
      } else {
        processError(&ctx, ident->name + L" is not defined", __FILE__, __func__,
                     __LINE__);
      }
    }
  } else if (Pointer *pointer = dynamic_cast<Pointer *>(&this->dest)) {
    eval::Ptr_Addr(&ctx, *pointer, 13);
  } else {
    processError(
        &ctx,
        util::str2wstr(typeid(this->dest).name()) + L" is not implemented...",
        __FILE__, __func__, __LINE__);
  }
  // Load value
  eval::Expr(&ctx, val, 14);

  if (op == L"++") {
    ctx.Asm->add(1, 14, 14);
  } else if (op == L"--") {
    ctx.Asm->add(-1, 14, 14);
  } else if (op == L"=") {
  } else if ((util::isMathOp(op[0]) ||
              util::isBitOpFull(op.substr(0, op.length() - 1))) &&
             op[op.length() - 1] == L'=') {
    op = op.substr(0, op.length() - 1);
    if (op == L"+") {
      ctx.Asm->addReg(14);
    } else if (op == L"-") {
      ctx.Asm->subReg(14);
    } else if (op == L"/") {
      ctx.Asm->divReg(14);
    } else if (op == L"*") {
      ctx.Asm->mulReg(14);
    } else if (op == L"%") {
      ctx.Asm->modReg(14);
    } else if (op == L"<<") {
      ctx.Asm->shtlReg(14);
    } else if (op == L">>") {
      ctx.Asm->shtrReg(14);
    } else {
      processError(&ctx, L"Unknown operator type: " + op, __FILE__, __func__,
                   __LINE__);
    }
  }
  if (Variable *ident = dynamic_cast<Variable *>(&this->dest)) {
    std::string target = util::wstr2str(ident->name);
    // check: is avail variable of target
    ctx.Asm->poke(ctx.variables[target].offset, 1, 14);
  } else if (dynamic_cast<Pointer *>(&this->dest)) {
    ctx.Asm->poke(0, 13, 14);
  } else {
    processError(
        &ctx,
        util::str2wstr(typeid(this->dest).name()) + L" is not implemented...",
        __FILE__, __func__, __LINE__);
  }
}