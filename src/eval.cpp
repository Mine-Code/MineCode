#include <eval.h>

#include <parserTypes.h>

using namespace parserTypes;

expr& optimize(expr& val){
    int immutable=0;
    for(auto part:val.parts){
        if(part.isSingle() && part.parts[0].isSingle() && part.parts[0].parts[0].type==power::IMM){
            // single pattern
            immutable+=part.parts[0].parts[0].imm;
        }
    }
    return val;
}

void eval::Expr (expr,int){

}
void eval::Expo (expo,int){

}
void eval::Term (term,int){

}
void eval::Power(power,int){

}