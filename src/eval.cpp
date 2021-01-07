#include <eval.h>

#include <parserTypes.h>

using namespace parserTypes;

term& optimize(term& val){
    int immutable=0;
    term newTerm;
    for(auto part:val.parts){
        if(part.isSingle() && part.parts[0].type==power::IMM){
            // single pattern
            immutable*=part.parts[0].imm;
        }else{
            newTerm.parts.emplace_back(part);
        }
    }
    if(immutable!=0){
        power value;
        value.type=power::IMM;
        value.imm=immutable;

        expo exponent;
        exponent.parts.emplace_back(value);

        val.parts.emplace_back(exponent);
    }
    return val;
}

expr& optimize(expr& val){
    int immutable=0;
    expr newExpr;
    for(auto part:val.parts){
        if(part.isSingle() && part.parts[0].isSingle() && part.parts[0].parts[0].type==power::IMM){
            // single pattern
            immutable+=part.parts[0].parts[0].imm;
        }else{
            newExpr.parts.emplace_back(part);
        }
    }
    if(immutable!=0){
        power value;
        value.type=power::IMM;
        value.imm=immutable;

        expo exponent;
        exponent.parts.emplace_back(value);

        term Term;
        Term.parts.emplace_back(exponent);

        val.parts.emplace_back(Term);
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