#include <mcl.h>

#include <fstream>

#include <parserTypes.h>

mcl::mcl(std::wstring name){
    std::ifstream file("file.json");
    file >> raw;
}

void operator<<(parserTypes::parserContext& ctx, mcl& pl){
    
}