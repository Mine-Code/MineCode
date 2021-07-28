#include <eval.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <syntaxError.h>
#include <util.h>

using namespace parserTypes;

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