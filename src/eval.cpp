#include <eval.h>

#include <parserTypes.h>

using namespace parserTypes;

expr& optimize(expr& val){
    int immutable=0;
    for(auto part:val.parts){
        if(part.isSingle()){
            // single pattern
            if(part.parts[0].isSingle()){
                power elem=part.parts[0].parts[0];
                if(elem.type==power::IMM){
                    immutable+=elem.imm;
                }
            }
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