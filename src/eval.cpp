#include <eval.h>

#include <parserTypes.h>
#include <syntaxError.h>
#include <stmtProcessor.h>
#include <util.h>

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
        stmtProcessor::executeFunction(ctx,*obj.func);
        ctx.Asm->moveResister(3,dest);
        break;
    case power::IMM:
        ctx.Asm->writeRegister(obj.imm,dest);
        break;
    case power::PTR:
        synErr::processError(ctx,L"Pointer isn't supported...",__FILE__,__func__,__LINE__);
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
void eval::Ptr  (parserContext& ctx,ptr obj,int dest){
    int offs=ctx.Asm->stack_offset;
    // calculate offset
    int offset=0;
    for(auto val:obj.offsets)offset+=val;
    // get value
    std::string key;
    switch (obj.base.type)
    {
    case ptrBase::IMM:
        ctx.Asm->peek_i(obj.base.imm,offset,dest);
        break;
    case ptrBase::IDENT:
        key = util::wstr2str(obj.base.ident);
        if(ctx.variables.count(key)==0){
            // doesn't have key
            synErr::processError(ctx,obj.base.ident+L" is not found!",__FILE__,__func__,__LINE__);
        }
        ctx.Asm->pop(ctx.variables[key].offset,14);
        ctx.Asm->peek(offset,dest,14);
        break;
    case ptrBase::PTR:
        Ptr(ctx,*obj.base.pointer,14);
        ctx.Asm->peek(offset,dest,14);
    default:
        break;
    }
    ctx.Asm->stack_offset=offs;
}