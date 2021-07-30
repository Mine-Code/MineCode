#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <syntaxError.h>
#include <util.h>

using namespace parserTypes;
using namespace parserTypes::expr;
void eval::Term(parserCore *that, term obj, int dest) {
  int offs = that->Asm->stack_offset;
  struct offset {
    enum Type { MUL, DIV, MOD };

    Type type;
    int offset;
  };
  std::vector<struct offset> stackOffsets;

  if (obj.parts.size() == 1) {
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