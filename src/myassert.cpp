#include <myassert.h>

#include <sstream>

#include <syntaxError.h>
#include <parserTypes.h>

void assert_fail(const char* fname,const char* funcname,int line,parserTypes::parserContext& ctx,wchar_t* msg){
    synErr::syntaxError(ctx,msg,fname,funcname,line);
}