#include <mcl.h>

#include <codecvt>
#include <fstream>
#include <locale>
#include <unordered_map>
#include <../../lib/json/single_include/nlohmann/json.hpp>

#include <parserTypes.h>
#include <parserWrap.h>
#include <parserCore.h>

using namespace std::literals::string_literals;
using json = nlohmann::json;

json convertTree2Single(json);

void operator<<(parserTypes::parserContext& ctx, std::string name){
    json j;
    std::ifstream jsonin;
    jsonin.open("mcls/"+name+".json");
    jsonin >> j;
    
    // load datas
    json puts=convertTree2Single(j["put"]);
    json pointers=convertTree2Single(j["pointers"]);
    json functions=convertTree2Single(j["functions"]);

    auto converter=std::wstring_convert<std::codecvt_utf8<wchar_t>>();
    parserWrap compile;
    
    parserTypes::parserContext bk=compile.ctx;

    // compile pointers
    for(auto [name,obj]: pointers.items()){
        std::wcout<<"setting of "<<converter.from_bytes(name)<<std::endl;
        std::cout<<obj;
    }

    // compile puts
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