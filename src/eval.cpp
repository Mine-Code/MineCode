#include <eval.h>

#include <parserTypes.h>

using namespace parserTypes;

expr& optimize(expr& val){
    int immutable=0;
    for(auto part:val.parts){
        if(part.isSingle() && part.parts[0].isSingle() && part.parts[0].parts[0].type==power::IMM){
            // single pattern
            power elem=part.parts[0].parts[0];
            immutable+=elem.imm;
        }
    }
    return val;
}

void eval::Expr (expr,int){

}
void eval::Expo (expo,int){

}
void eval::Power(power,int){

}