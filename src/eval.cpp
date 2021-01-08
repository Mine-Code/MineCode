#include <eval.h>

#include <parserTypes.h>
#include <syntaxError.h>

using namespace parserTypes;

expo& optimize(expo& val){
    return val;
}

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

    val.parts_mul=newTerm.parts_mul;
    val.parts_div=newTerm.parts_div;
    val.parts_mod=newTerm.parts_mod;

    return val;
}

expr& optimize(expr& val){
    int immutable=0;
    expr newExpr;
    for(auto _part:val.parts){
        auto part=optimize(_part);
        if(part.isSingle() && part.parts_mul[0].isSingle() && part.parts_mul[0].parts[0].type==power::IMM){
            immutable+=part.parts_mul[0].parts[0].imm;
        }
        else{
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
        Term.parts_mul.emplace_back(exponent);

        val.parts.emplace_back(Term);
    }

    val.parts = newExpr.parts; // copy new expr (stack safe)
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
void eval::Power(parserContext& ctx,power obj,int dest){
    int offs=ctx.Asm->stack_offset;
    switch (obj.type)
    {
    case power::EXPR:
        Expr(ctx,obj.expr,dest);
        break;
    case power::FLT:
        synErr::processError(ctx,L"Float isn't supported...",__FILE__,__func__,__LINE__);
        break;
    case power::FUNCCALL:
        synErr::processError(ctx,L"FuncCall isn't supported...",__FILE__,__func__,__LINE__);
        break;
    default:
        synErr::processError(ctx,
            L"unknown type error ["
                +std::to_wstring(obj.type)
            +L"]"
            ,__FILE__,__func__,__LINE__
        );
        break;
    }
    ctx.Asm->stack_offset=offs;
}