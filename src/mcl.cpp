#include <mcl.h>

#include <fstream>
#include <locale>
#include <unordered_map>

#include <parserTypes.h>

using namespace std::literals::string_literals;

json convertTree2Single(json);

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
    json puts=convertTree2Single(j["put"]);
    for(auto [key,val]: puts.items()){
        
    }
}

bool isShallow(json src){
    for(auto [key,val]:src.items()){
        if(val.size()==1){
            return false;
        }
    }
    return true;
}

json convertTree2Single(json src){
    json dest;
    if(isShallow(src))return src;
    else{
        for(auto [key,val]: src.items()){
            json tmp = convertTree2Single(val);
            for (auto [cKey,cVal]: tmp.items())
            {
                dest[key+"."+cKey]=cVal;
            }
            
        }
    }
    return dest;
}