#include <parserContext.h>

parserCtx::parserContext::parserContext()
{
    stream=streamType();
    iter=iterType();
    variables=varsType();
}