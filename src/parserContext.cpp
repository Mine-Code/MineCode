#include <parserContext.h>

parserCtx::parserContext::parserContext()
{
    Asm=new Assembly();
    iter=iterType();
    variables=varsType();
}