#include <parserCore.h>
#include <parserTypes.h>
#include <syntaxError.h>

using namespace synErr;

void synErr::ReportError(parserCore *that, errorType type, std::wstring exMsg,
                         const char *fname, const char *funcname, int line) {
  std::wstring strType;
  if (type == SYNTAX) {
    strType = L"syntax";
  } else if (type == TOKENIZE) {
    strType = L"tokenize";
  } else if (type == PROCESS) {
    strType = L"process";
  }

  std::wstring prev = that->iter.peekSafe(-1);
  std::wstring curr = that->iter.peekSafe(0);
  std::wstring next = that->iter.peekSafe(1);
  std::wstring pos = prev + curr + next;

  std::wstring offset = std::wstring(prev.length(), L' ');
  std::wstring uline = std::wstring(curr.length(), L'^');
  std::wcout << "-------Error--------" << std::endl
             << "| type:" << strType << std::endl;
  if (exMsg != L"") std::wcout << "| msg :" << exMsg << std::endl;
  std::wcout << "| Where?" << std::endl
             << "|  func: " << funcname << std::endl
             << "|  copyable: " << fname << ":" << line << std::endl
             << "| Pos Informations" << std::endl
             << "| " << pos << std::endl
             << "| " << offset << uline << std::endl;
  std::wcout << "--------------------" << std::endl;

  throw exMsg;
}
void synErr::syntaxError(parserCore *that, std::wstring ex, const char *fname,
                         const char *funcname, int line) {
  ReportError(that, SYNTAX, ex, fname, funcname, line);
}
void synErr::tokenizeError(parserCore *that, std::wstring ex, const char *fname,
                           const char *funcname, int line) {
  ReportError(that, TOKENIZE, ex, fname, funcname, line);
}
void synErr::processError(parserCore *that, std::wstring ex, const char *fname,
                          const char *funcname, int line) {
  ReportError(that, PROCESS, ex, fname, funcname, line);
}