#include <eval.h>

#include <parserTypes.h>

using namespace parserTypes;

expr& optimize(expr& val){
    int immutable=0;
    for(auto part:val.parts){
        if(part.isSingle()){
            // single pattern
            if(part.parts[0].isSingle()){
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