#include <mcl.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <parserWrap.h>
#include <util.h>

#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>

using namespace std::literals::string_literals;
using json = nlohmann::json;

json convertTree2Single(json);
json convertTree2Single_function(json);

void operator<<(parserWrap &ctx, std::string name) {
  json j;
  std::ifstream jsonin;
  jsonin.open("../mcls/" + name + ".json");
  jsonin >> j;

  // load datas
  json puts = convertTree2Single(j["put"]);
  json pointers = j["pointers"];
  json functions = convertTree2Single_function(j["functions"]);
  std::wstring pointerasm;

  // check defined compile
  if (!ctx.ctx.compiler) {
    ctx.ctx.compiler = new parserWrap;
  }

  // output Lib start
  ctx.ctx.stream << "# Lib:" << util::str2wstr(name) << std::endl;

  // compile pointers
  ctx.ctx.compiler->ctx.Asm = new Assembly(ctx.ctx.compiler->ctx.stream);
  ctx.ctx.compiler->ctx.Asm->stack_offset =
      ctx.ctx.Asm->stack_offset;  // copy stack_offset
  for (auto pointer : j["pointers"]) {
    auto name = pointer[0].get<std::string>();
    auto expr = pointer[1].get<std::string>();
    ctx.ctx.stream << "# Pointer " << util::str2wstr(name) << " in "
                   << util::str2wstr(name) << std::endl;

    std::wstring source = util::str2wstr(name + " = " + expr);

    ctx.ctx.stream << ctx.ctx.compiler->compile(source);
  }
  ctx.ctx.Asm->stack_offset =
      ctx.ctx.compiler->ctx.Asm->stack_offset;  // set stack_offset
  for (auto [key, val] : ctx.ctx.compiler->ctx.variables) {
    ctx.ctx.variables[key] = val;
  }

  for (auto [name, obj] : puts.items()) {
    auto type = obj["type"].get<std::string>();
    auto proc = obj["proc"].get<std::string>();
    ctx.ctx.puts[name] = proc;
    ctx.ctx.puts_table[name] = type == "MineCode" ? 1 : 0;
  }

  for (auto [name, obj] : functions.items()) {
    parserTypes::function func;
    func.addr = (uint32_t)obj["addr"].get<int>();

    for (auto arg : obj["args"]) {
      parserTypes::funcArg newarg;

      std::string type = arg["T"];
      if (type == "int") {
        newarg.type = parserTypes::funcArgType::INT;
      } else if (type == "cstr") {
        newarg.type = parserTypes::funcArgType::CSTR;
      } else if (type == "wstr") {
        newarg.type = parserTypes::funcArgType::WSTR;
      } else if (type == "ptr") {
        newarg.type = parserTypes::funcArgType::PTR;
      }
      ctx.ctx.compiler->set(util::str2wstr(arg["default"]));
      ctx.ctx.compiler->tokenize();
      // newarg.defaultValue=parserCore::expr(ctx.ctx.compiler->ctx);
    }
    ctx.ctx.functions[name] = func;
  }
  ctx.ctx.stream << "##Lib:" << util::str2wstr(name) << "\n";
}

bool isShallow(json src) {
  for (auto [key, val] : src.items()) {
    if (val.is_object()) {
      return false;
    }
  }
  return true;
}

json convertTree2Single(json src) {
  json dest;
  if (isShallow(src)) {
    return src;
  } else {
    for (auto [key, val] : src.items()) {
      json tmp = convertTree2Single(val);
      if (isShallow(tmp)) {
        dest[key] = tmp;
      } else {
        for (auto [cKey, cVal] : tmp.items()) {
          dest[key + "." + cKey] = cVal;
        }
      }
    }
  }
  return dest;
}
bool isShallow_function(json src) {
  for (auto [key, val] : src.items()) {
    if (key == "addr") {
      return true;
    }
  }
  return false;
}
json convertTree2Single_function(json src) {
  json dest;
  if (isShallow_function(src)) {
    return src;
  } else {
    for (auto [key, val] : src.items()) {
      json tmp = convertTree2Single_function(val);
      if (isShallow_function(tmp)) {
        dest[key] = tmp;
      } else {
        for (auto [cKey, cVal] : tmp.items()) {
          dest[key + "::" + cKey] = cVal;
        }
      }
    }
  }
  return dest;
}