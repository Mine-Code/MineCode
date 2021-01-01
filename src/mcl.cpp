#include <mcl.h>

#include <fstream>

#include <parserTypes.h>

using namespace std::literals::string_literals;
mcl::mcl(std::wstring name){
    std::ifstream file("file.json");
    file >> raw;
}

void operator<<(parserTypes::parserContext& ctx, mcl pl){
    
}