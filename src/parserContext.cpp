#include <parserContext.h>

parserCtx::parserContext::parserContext()
{
    stream=streamType();
    Asm=new Assembly(stream);
    iter=iterType();
    variables=varsType();
}