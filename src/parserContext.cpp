#include <parserContext.h>

parserCtx::parserContext::parserContext()
{
    std::wstringstream st;
    Asm=new Assembly(st);
    iter=iterType();
    variables=varsType();
}