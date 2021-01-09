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
    : args( std::vector<expr>() )
    , funcId( L"0x00000000" )
    , type( ADDRESS )
{

}

parserTypes::power::power()
    : type(power::IMM)
    , imm(0)
{
}

bool parserTypes::expr::isSingle(){return parts.size()==1;}
bool parserTypes::expo::isSingle(){return parts.size()==1;}
bool parserTypes::term::isSingle(){return parts.size()==1;}