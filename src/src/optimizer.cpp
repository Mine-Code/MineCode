#include <optimizer.h>

#include <typedIterator.hxx>

optimizer::optimizer(std::wstring src) {
  std::wstring line;
  for (auto ch : src) {
    line += ch;
    if (ch == '\n') {
      codes.emplace_back(line);
      line = L"";
    }
  }
}

std::vector<std::wstring> optimizer::get() {
  std::vector<std::wstring> ret;

  return ret;
}