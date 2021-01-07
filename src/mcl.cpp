#include <mcl.h>

#include <codecvt>
#include <fstream>
#include <locale>
#include <unordered_map>

#include <parserTypes.h>
#include <parserWrap.h>

using namespace std::literals::string_literals;
using json = nlohmann::json;

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

void operator<<(parserTypes::parserContext& ctx, std::string name){
    json j=pl.raw;
    
    // load datas
    json puts=convertTree2Single(j["put"]);
    json pointers=convertTree2Single(j["pointers"]);
    json functions=convertTree2Single(j["functions"]);

    // compile puts
    parserWrap compile;
    auto converter=std::wstring_convert<std::codecvt_utf8<wchar_t>>();
    for(auto [name,obj]: puts.items()){
        std::wcout<<"compile of "<<converter.from_bytes(name)<<std::endl;
        compile.set(converter.from_bytes(obj["proc"].get<std::string>()));
        compile.tokenize();
        auto compiled=compile.compile();
        ctx.puts[name]=converter.to_bytes(compiled);
    }
}

bool isShallow(json src){
    for(auto [key,val]:src.items()){
        if(val.is_object()){
            return false;
        }
    }
    return true;
}

json convertTree2Single(json src){
    json dest;
    if(isShallow(src)){
        return src;
    }else{
        for(auto [key,val]: src.items()){
            json tmp = convertTree2Single(val);
            if(isShallow(tmp)){
                dest[key]=tmp;
            }else{
                for (auto [cKey,cVal]: tmp.items()){
                    dest[key+"."+cKey]=cVal;
                }
            }
            
        }
    }
    return dest;
}