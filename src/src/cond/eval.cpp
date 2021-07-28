#include <cond/eval.h>
#include <cond/util.h>
#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <syntaxError.h>
#include <util.h>

#include <primary/all.hpp>

using namespace parserTypes;

void condeval::EvalCond(parserCore* that, Cond cond) {
  std::wstring footer = std::to_wstring(that->Asm->make_if_ctr++);
  std::wstring label = L"if_" + footer;
  std::wstring endLabel = L"endif_" + footer;
  for (auto condChild : cond.conds) {
    CondAnd(that, condChild, label);
  }
  that->Asm->Jump(endLabel);
  that->Asm->makeLabel(label);
}
void condeval::EvalCond(parserCore* that, Cond cond, std::wstring trueLabel,
                        std::wstring falseLabel) {
  if (trueLabel == L"" && falseLabel != L"") {
    EvalCond(that, util::condAnd2cond(util::invertConditional(cond)),
             falseLabel, L"");
    return;
  }
  for (auto condChild : cond.conds) {
    CondAnd(that, condChild, trueLabel);
  }
  if (falseLabel != L"") {
    that->Asm->Jump(falseLabel);
  }
}
void condeval::CondAnd(parserCore* that, condAnd cond, std::wstring target) {
  if (cond.conds.size() == 1) {
    CondChild(that, cond.conds[0], target);
  } else {
    std::wstring elseLabel = that->Asm->getLabel();
    for (auto child : cond.conds) {
      CondChild(that, util::invertConditional(child), elseLabel);
    }
    that->Asm->Jump(target);
    that->Asm->makeLabel(elseLabel);
  }
}
void condeval::CondChild(parserCore* that, condChild cond,
                         std::wstring target) {
  using namespace parserTypes::primary;
  if (cond.op == condChild::SINGLE || cond.op == condChild::SINGLE_INV) {
    varType var;
    unsigned int compareTarget = 1;
    if (cond.op == condChild::SINGLE_INV) {
      compareTarget = 0;
    }
    if (Variable* ident = dynamic_cast<Variable*>(cond.single)) {
      var = that->variables[util::wstr2str(ident->name)];
      if (var.type == varType::INT) {
        that->Asm->pop(var.offset);
        that->Asm->compareImm(13, compareTarget);
        that->Asm->condJump(Assembly::EQU, 0, target);
      } else {
        synErr::processError(
            that, L"condition variable is must be integer. but this is float",
            __FILE__, __func__, __LINE__);
      }
    } else if (Immutable* imm = dynamic_cast<Immutable*>(cond.single)) {
      if (imm->value == compareTarget) {
        that->stream << "b " << target << "\n";
      }
    } else if (Pointer* ptr = dynamic_cast<Pointer*>(cond.single)) {
      eval::Ptr(that, ptr->pointer);
      that->Asm->compareImm(13, compareTarget);
    } else if (dynamic_cast<Str*>(cond.single)) {
      synErr::processError(
          that, L"condition value is must be integer, identity or pointer",
          __FILE__, __func__, __LINE__);
    }
  } else if (cond.op == condChild::COND) {
    EvalCond(that, cond.child, target, L"");
  } else {
    // process val1/2
    eval::Expr(that, cond.val1, 13);
    eval::Expr(that, cond.val2, 14);
    that->Asm->compare(13, 14);
    Assembly::condType t;
    switch (cond.op) {
      case condChild::EQU:
        t = Assembly::EQU;
        break;
      case condChild::NEQ:
        t = Assembly::NEQ;
        break;
      case condChild::LT:
        t = Assembly::LT;
        break;
      case condChild::GT:
        t = Assembly::GT;
        break;
      case condChild::GE:
        t = Assembly::GE;
        break;
      case condChild::LE:
        t = Assembly::LE;
        break;
      default:
        std::wcout << "Warning: invalid conditional type id (set to equal)"
                   << std::endl;
        t = Assembly::EQU;
        break;
    }
    that->Asm->condJump(t, 0, target);
  }
}