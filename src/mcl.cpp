#include <mcl.h>

#include <fstream>

#include <parserTypes.h>

using namespace std::literals::string_literals;
mcl::mcl(std::wstring name){
    std::wstring path;
    path+=L"mcls/";
    path+=name;
    path+=L".json";
    std::ifstream file(std::string(path));
    file >> raw;
}

void operator<<(parserTypes::parserContext& ctx, mcl pl){
    
}