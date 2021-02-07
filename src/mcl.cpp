#include <mcl.h>

#include <fstream>
#include <unordered_map>
#include <../../lib/json/single_include/nlohmann/json.hpp>

#include <parserTypes.h>
#include <parserWrap.h>
#include <parserCore.h>
#include <util.h>

using namespace std::literals::string_literals;
using json = nlohmann::json;

json convertTree2Single(json);
json convertTree2Single_function(json);

std::wstring convPut(std::wstring);

void operator<<(parserWrap& ctx, std::string name){
    json j;
    std::ifstream jsonin;
    jsonin.open("mcls/"+name+".json");
    jsonin >> j;
    
    // load datas
    json puts=convertTree2Single(j["put"]);
    json pointers=j["pointers"];
    json functions=convertTree2Single_function(j["functions"]);
    std::wstring pointerasm;

    parserWrap compiler;

    // compile pointers
    compiler.ctx.Asm->stack_offset=ctx.ctx.Asm->stack_offset; // copy stack_offset
    for(auto pointer: j["pointers"]){
        auto name=pointer[0].get<std::string>();
        auto expr=pointer[1].get<std::string>();
        std::wcout<<"setting of "<<util::str2wstr(name)<<std::endl;
        
        std::wstring source=util::str2wstr(name+" = "+expr);
        
        pointerasm += compiler.compile(source);
    }
    ctx.ctx.Asm->stack_offset=compiler.ctx.Asm->stack_offset; // set stack_offset
    ctx.ctx.variables=compiler.ctx.variables;

    compiler.reset();
    for(auto [name,obj]: puts.items()){
        std::wcout<<"compile of "<<util::str2wstr(name)<<std::endl;

        auto type=obj["type"].get<std::string>();
        auto proc=obj["proc"].get<std::string>();
        if(type=="MineCode"){
            std::wstring source=util::str2wstr(proc);
            source=convPut(source);
            ctx.ctx.puts[name]=util::wstr2str(compiler.compile(source));
        }else if(type=="asm"){
            ctx.ctx.puts[name]=proc;
        }
    }

    for(auto [name,obj]: functions.items()){
        std::wcout<<"converting of "<<util::str2wstr(name)<<std::endl;

        parserTypes::function func;
        func.addr=(uint32_t)obj["addr"].get<int>();

        for(auto arg: obj["args"]){
            parserTypes::funcArg newarg;
            
            std::string type=arg["T"];
            if(type=="int"){
                newarg.type=parserTypes::funcArgType::INT;
            }else if(type=="cstr"){
                newarg.type=parserTypes::funcArgType::CSTR;
            }else if(type=="wstr"){
                newarg.type=parserTypes::funcArgType::WSTR;
            }else if(type=="ptr"){
                newarg.type=parserTypes::funcArgType::PTR;
            }
            compiler.set(util::str2wstr(arg["default"]));
            compiler.tokenize();
            newarg.defaultValue=parserCore::expr(compiler.ctx);
        }
        ctx.ctx.functions[name]=func;
    }
    std::wcout<<"compiled all"<<std::endl;
    ctx.ctx.stream
        << "# Lib:"<< util::str2wstr(name)<<"\n"
        << pointerasm
        << "##Lib:"<< util::str2wstr(name)<<"\n";
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
bool isShallow_function(json src){
    for(auto [key,val]:src.items()){
        if(key=="addr"){
            return true;
        }
    }
    return false;
}
json convertTree2Single_function(json src){
    json dest;
    if(isShallow_function(src)){
        return src;
    }else{
        for(auto [key,val]: src.items()){
            json tmp = convertTree2Single_function(val);
            if(isShallow_function(tmp)){
                dest[key]=tmp;
            }else{
                for (auto [cKey,cVal]: tmp.items()){
                    dest[key+"::"+cKey]=cVal;
                }
            }
            
        }
    }
    return dest;
}
std::wstring convPut(std::wstring src){
    auto iter=iterator<wchar_t>(util::convToVector<wchar_t,std::wstring>(src));
}