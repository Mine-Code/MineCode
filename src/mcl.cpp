#include <mcl.h>

#include <fstream>
#include <locale>

#include <parserTypes.h>

using namespace std::literals::string_literals;
mcl::mcl(std::string name){
    std::string path;
    path+="mcls/";
    path+=name;
    path+=".json";
    std::ifstream jsonin;
    jsonin.open(path);
    jsonin >> raw;
}

void operator<<(parserTypes::parserContext& ctx, mcl pl){
    json j=pl.raw;
    
    // load put
    json puts=j["put"];
}

json convertTree2Single(json src){
    json dest;


    return dest
}