#include <mcl.h>

#include <fstream>
#include <locale>
#include <unordered_map>

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

bool isShallow(json src){
    for(auto [key,val]:src.items()){
        if(val.is_object()){
            return false;
        }
    }
    return true;
}

std::unordered_map<std::wstring,json> convertTree2Single(json src){
    json dest;


    return dest;
}