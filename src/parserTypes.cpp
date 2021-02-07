#include <parserTypes.h>

parserTypes::parserContext::parserContext()
{
    stream=streamType();
    Asm=new Assembly(stream);
    iter=iterType();
    variables=varsType();
    compiler=nullptr;
}

parserTypes::parserContext::parserContext(parserTypes::parserContext& source)
    : Asm(source.Asm)
    , iter(source.iter)
    , variables(source.variables)
    , puts(source.puts)
    , compiler(nullptr)
{
    stream<<source.stream.rdbuf();
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