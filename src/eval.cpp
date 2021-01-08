#include <eval.h>

#include <parserTypes.h>

using namespace parserTypes;

term& optimize(term& val){
    int immutable_mul=0;
    term newTerm;
    // process of mul
    for(auto part:val.parts_mul){
        if(part.isSingle() && part.parts[0].type==power::IMM){
            // single pattern
            immutable_mul*=part.parts[0].imm;
        }else{
            newTerm.parts_mul.emplace_back(part);
        }
    }
    return val;
}

expr& optimize(expr& val){
    int immutable_mul=0;
    int immutable_div=0;
    int immutable_mod=0;
    expr newExpr;
    for(auto part:val.parts){
        if(part.isSingle()){
            if(part.parts_mul[0].isSingle() &&part.parts_mul[0].parts[0].type==power::IMM){
                immutable_mul+=part.parts_mul[0].parts[0].imm;
            }else if(part.parts_div[0].isSingle() &&part.parts_div[0].parts[0].type==power::IMM){
                immutable_div+=part.parts_div[0].parts[0].imm;
            }else if(part.parts_mod[0].isSingle() &&part.parts_mod[0].parts[0].type==power::IMM){
                immutable_mod+=part.parts_mod[0].parts[0].imm;
            }else{
                newExpr.parts.emplace_back(part);
            }
        }
        else{
            newExpr.parts.emplace_back(part);
        }
    }
    if(immutable_mul!=0){
        power value;
        value.type=power::IMM;
        value.imm=immutable_mul;

        expo exponent;
        exponent.parts.emplace_back(value);

        term Term;
        Term.parts_mul.emplace_back(exponent);

        val.parts.emplace_back(Term);
    }
    return val;
}

void eval::Expr (parserContext& ctx,expr,int){
    int offs=ctx.Asm->stack_offset;

    ctx.Asm->stack_offset=offs;
}
void eval::Expo (parserContext& ctx,expo,int){
    int offs=ctx.Asm->stack_offset;

    ctx.Asm->stack_offset=offs;
}
void eval::Term (parserContext& ctx,term,int){
    int offs=ctx.Asm->stack_offset;

    ctx.Asm->stack_offset=offs;
}
void eval::Power(parserContext& ctx,power,int){
    int offs=ctx.Asm->stack_offset;

    ctx.Asm->stack_offset=offs;
}