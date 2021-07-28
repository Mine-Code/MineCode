#include <parserCore.h>
#include <parserWrap.h>
#include <syntaxError.h>
#include <util.h>

#include <stmt/put.hpp>
#include <string>
#include <typedIterator.hxx>

std::wstring convPut(std::wstring, std::wstring);

parserTypes::stmt::Put::~Put() {}
void parserTypes::stmt::Put::exec(parserCore& ctx) {
  std::wstring source = util::str2wstr(ctx.puts[dest]);
  source = convPut(source, val);
  if (ctx.puts_table[dest]) {
    ctx.stream << ctx.compiler->compile(source);
  } else {
    ctx.stream << source;
  }
}
std::wstring convPut(std::wstring src, std::wstring argument) {
  std::wstring ret;
  auto iter = iterator<wchar_t>(util::convToVector<wchar_t, std::wstring>(src));
  while (iter.hasData()) {
    wchar_t ch = iter.next();
    wchar_t ne = iter.peekSafe(1);
    if (ch == '$' && ne == '$') {
      ret += '$';
      iter.next();
    } else if (ch == '$' && iter.peekSafe(0) == 'a' &&
               iter.peekSafe(1) == 'r' && iter.peekSafe(2) == 'g') {
      for (auto ch : argument) {
        ret += ch;
      }
      iter.index += 3;
    } else {
      ret += ch;
    }
  }
  return ret;
}