#include <mcl.h>

#include <fstream>

#include <parserTypes.h>

using namespace std::literals::string_literals;
mcl::mcl(std::string name){
    std::string path;
    path+="mcls/";
    path+=name;
    path+=".json";
    std::ifstream jsonin;
    jsonin.open(path);
}

void operator<<(parserTypes::parserContext& ctx, mcl pl){
    
}