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

parserTypes::ExecFunc::ExecFunc()
    : args( std::vector<std::wstring>() )
    , funcId( L"0x00000000" )
{

}