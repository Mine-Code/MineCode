#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <syntaxError.h>
#include <util.h>

#include <expr/term.hpp>

using namespace parserTypes;
using namespace parserTypes::expr;
parserTypes::expr::term::~term() {}
parserTypes::expr::BaseExpr& parserTypes::expr::term::optimize() {
  return *this;
}
void parserTypes::expr::term::eval(parserCore& ctx, int dest) {
  int offs = ctx.Asm->stack_offset;
  struct offset {
    enum Type { MUL, DIV, MOD };

    Type type;
    int offset;
  };
  std::vector<struct offset> stackOffsets;

  if (this->parts.size() == 1) {
    this->parts[0].value.eval(ctx, dest);
  } else {
    // write all
    for (auto elem : this->parts) {
      elem.value.eval(ctx, dest);

      int offset = ctx.Asm->push(dest);
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
          synErr::processError(&ctx, L"Unknown expr_wrap type ", __FILE__,
                               __func__, __LINE__);
          std::terminate();  // dead code
      }

      struct offset element;
      element.type = type;
      element.offset = offset;

      stackOffsets.emplace_back(element);
    }

    ctx.Asm->writeRegister(1, dest);  // init dest
    for (auto a : stackOffsets) {
      ctx.Asm->pop(a.offset, 14);
      switch (a.type) {
        case offset::MUL:
          ctx.stream << "mullw r" << dest << ", r" << dest << ", r14"
                     << std::endl;
          break;
        case offset::DIV:
          ctx.stream << "divw r" << dest << ", r" << dest << ", r14"
                     << std::endl;
          break;
        case offset::MOD:
          ctx.stream << "#mod r" << dest << ", r" << dest << ", r14"
                     << std::endl;
          break;
      }
    }
  }

  ctx.Asm->stack_offset = offs;
}