#include <parserTypes.h>

parserTypes::parserContext::parserContext()
{
    stream=streamType();
    Asm=new Assembly(stream);
    iter=iterType();
    variables=varsType();
}
parserTypes::cond::cond()
    : conds( std::vector<std::pair<Type,condChild>>() )
{
    
}