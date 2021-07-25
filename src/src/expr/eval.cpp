#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <stmtProcessor.h>
#include <syntaxError.h>
#include <util.h>

using namespace parserTypes;

void Ptr_AddrBase(parserCore *that, ptr obj, int dest) {
  eval::Expr(that, obj.getBase(), dest);
}

void eval::Expr(parserCore *that, expr obj, int dest) {
  int offs = that->Asm->stack_offset;
  std::vector<int> stackOffsets;

  if (obj.isSingle()) {
    Term(that, obj.parts[0], dest);
  } else {
    // write all
    for (auto elem : obj.parts) {
      Term(that, elem, dest);
      stackOffsets.emplace_back(that->Asm->push(dest));
    }

    that->Asm->writeRegister(0, dest);
    for (auto i : stackOffsets) {
      that->Asm->pop(i, 14);
      that->stream << "add r" << dest << ", r" << dest << ", r14" << std::endl;
    }
  }

  that->Asm->stack_offset = offs;
}
void eval::Expo(parserCore *that, expo obj, int dest) {
  int offs = that->Asm->stack_offset;
  std::vector<int> stackOffsets;

  if (obj.isSingle()) {
    Power(that, obj.parts[0], dest);
  } else {
    // write all
    for (auto elem : obj.parts) {
      Power(that, elem, dest);
      stackOffsets.emplace_back(that->Asm->push(dest));
    }
    // TODO: power all stackOffsets
  }
  that->Asm->stack_offset = offs;
}
void eval::Term(parserCore *that, term obj, int dest) {
  int offs = that->Asm->stack_offset;
  struct offset {
    enum Type { MUL, DIV, MOD };

    Type type;
    int offset;
  };
  std::vector<struct offset> stackOffsets;

  if (obj.isSingle()) {
    Expo(that, obj.parts[0].value, dest);
  } else {
    // write all
    for (auto elem : obj.parts) {
      Expo(that, elem.value, dest);

      int offset = that->Asm->push(dest);
      offset::Type type;
      switch (elem.type) {
        case expo_wrap::MUL:
          type = offset::Type::MUL;
          break;
        case expo_wrap::DIV:
          type = offset::Type::DIV;
          break;
        case expo_wrap::MOD:
          type = offset::Type::MOD;
          break;
        default:
          synErr::processError(that, L"Unknown expr_wrap type ", __FILE__,
                               __func__, __LINE__);
          std::terminate();  // dead code
      }

      struct offset element;
      element.type = type;
      element.offset = offset;

      stackOffsets.emplace_back(element);
    }

    that->Asm->writeRegister(1, dest);  // init dest
    for (auto a : stackOffsets) {
      that->Asm->pop(a.offset, 14);
      switch (a.type) {
        case offset::MUL:
          that->stream << "mullw r" << dest << ", r" << dest << ", r14"
                       << std::endl;
          break;
        case offset::DIV:
          that->stream << "divw r" << dest << ", r" << dest << ", r14"
                       << std::endl;
          break;
        case offset::MOD:
          that->stream << "#mod r" << dest << ", r" << dest << ", r14"
                       << std::endl;
          break;
      }
    }
  }

  that->Asm->stack_offset = offs;
}
void eval::Power(parserCore *that, power obj, int dest) {
  int offs = that->Asm->stack_offset;
  switch (obj.type) {
    case power::EXPR:
      Expr(that, obj.expr, dest);
      break;
    case power::FLT:
      synErr::processError(that, L"Float isn't supported...", __FILE__,
                           __func__, __LINE__);
      break;
    case power::FUNCCALL:
      stmtProcessor::executeFunction(that, *obj.func);
      that->Asm->moveResister(3, dest);
      break;
    case power::IMM:
      that->Asm->writeRegister(obj.imm, dest);
      break;
    case power::PTR:
      Ptr(that, obj.Pointer, dest);
      break;
    case power::VAR:
      Var(that, obj.var, dest);
      break;
    default:
      synErr::processError(
          that, L"unknown type error [" + std::to_wstring(obj.type) + L"]",
          __FILE__, __func__, __LINE__);
      break;
  }
  that->Asm->stack_offset = offs;
}
void eval::Ptr(parserCore *that, ptr obj, int dest) {
  int offs = that->Asm->stack_offset;
  Ptr_AddrBase(that, obj, dest);
  that->Asm->peek(0, dest, dest);
  that->Asm->stack_offset = offs;
}
void eval::Ptr_Addr(parserCore *that, ptr obj, int dest) {
  Ptr_AddrBase(that, obj, dest);
}
void eval::Var(parserCore *that, std::wstring obj, int dest) {
  if (obj[0] == '\"') {
    that->Asm->setString(obj, dest);
  } else if (obj == L"false") {
    that->Asm->writeRegister(0, dest);
  } else if (obj == L"true") {
    that->Asm->writeRegister(1, dest);
  } else {
    if (that->variables.count(util::wstr2str(obj)) == 1) {
      that->Asm->pop(that->variables[util::wstr2str(obj)].offset, dest);
    } else {
      synErr::processError(that, L"variable not found: " + obj, __FILE__,
                           __func__, __LINE__);
    }
  }
}