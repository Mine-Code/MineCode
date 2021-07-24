#include <myassert.h>
#include <parserTypes.h>
#include <syntaxError.h>

#include <sstream>

void assert_fail(const char *fname, const char *funcname, int line,
                 parserCore *ctx, const wchar_t *msg) {
  synErr::syntaxError(ctx, msg, fname, funcname, line);
}