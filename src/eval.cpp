#include <eval.h>

#include <parserTypes.h>
#include <syntaxError.h>
#include <stmtProcessor.h>
#include <util.h>

using namespace parserTypes;


term& optimize(term& val){
    int immutable_mul=0;
    int immutable_div=0;
    term newTerm;
    // process of mul
    for(auto part:val.parts){
        if(part.value.isSingle() && part.value.parts[0].type==power::IMM){
            auto imm=part.value.parts[0].imm;
            if(part.type==expo_wrap::MUL)immutable_mul*=imm; else
            if(part.type==expo_wrap::DIV)immutable_div*=imm; else
            if(part.type==expo_wrap::MOD)newTerm.parts.emplace_back(part);
        }else{
            newTerm.parts.emplace_back(part);
        }
    }
    
    val.parts=newTerm.parts;

    // add mul/div element
    {// add mul
        power powerElem;
        powerElem.type=power::IMM;
        powerElem.imm=immutable_mul;

        expo expoElem;
        expoElem.parts.emplace_back(powerElem);

        expo_wrap wrapElem;
        wrapElem.type=expo_wrap::MUL;
        wrapElem.value=expoElem;

        val.parts.emplace_back(wrapElem);
    }
    {// add div
        power powerElem;
        powerElem.type=power::IMM;
        powerElem.imm=immutable_div;

        expo expoElem;
        expoElem.parts.emplace_back(powerElem);

        expo_wrap wrapElem;
        wrapElem.type=expo_wrap::DIV;
        wrapElem.value=expoElem;

        val.parts.emplace_back(wrapElem);
    }

    return val;
}

expr& optimize(expr& val){
    int immutable=0;
    expr newExpr;
    for(auto _part:val.parts){
        auto part=optimize(_part);
        if(part.isSingle() && part.parts[0].value.isSingle() && part.parts[0].value.parts[0].type==power::IMM){
            immutable+=part.parts[0].value.parts[0].imm;
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

        expo_wrap wrap;
        wrap.type=expo_wrap::MUL;
        wrap.value= exponent;

        term Term;
        Term.parts.emplace_back(wrap);

        val.parts.emplace_back(Term);
    }

    val.parts = newExpr.parts; // copy new expr (stack safe)
    return val;
}

void eval::Expr (parserContext& ctx,expr obj,int dest){
    int offs=ctx.Asm->stack_offset;
    std::vector<int> stackOffsets;

    if(obj.isSingle()){
        Term(ctx,obj.parts[0],dest);
    }else{
        // write all
        for(auto elem : obj.parts){
            Term(ctx,elem,dest);
            stackOffsets.emplace_back(ctx.Asm->push(dest));
        }

        ctx.Asm->writeRegister(0,dest);
        for (auto i : stackOffsets)
        {
            ctx.Asm->pop(i,14);
            // TODO: dest=dest+r14
            ctx.stream<<"# r"<<dest<<" = r"<<dest<<" + r14"<<std::endl;
        }
    }
    

    ctx.Asm->stack_offset=offs;
}
void eval::Expo (parserContext& ctx,expo obj,int dest){
    int offs=ctx.Asm->stack_offset;
    std::vector<int> stackOffsets;

    // write all
    for(auto elem : obj.parts){
        Power(ctx,elem,dest);
        stackOffsets.emplace_back(ctx.Asm->push(dest));
    }
    // TODO: power all stackOffsets
    ctx.Asm->stack_offset=offs;
}
void eval::Term (parserContext& ctx,term obj,int dest){
    int offs=ctx.Asm->stack_offset;
    struct offset{
        enum Type{
            MUL,
            DIV,
            MOD
        };

        Type type;
        int offset=0;
    };
    std::vector<struct offset> stackOffsets;

    if(obj.isSingle()){
        Expo(ctx,obj.parts[0].value,dest);
    }else{
        // write all
        for(auto elem : obj.parts){
            Expo(ctx,elem.value,dest);

            int offset=ctx.Asm->push(dest);
            offset::Type type;
            switch(elem.type)
            {
                case expo_wrap::MUL: type=offset::Type::MUL; break;
                case expo_wrap::DIV: type=offset::Type::DIV; break;
                case expo_wrap::MOD: type=offset::Type::MOD; break;
                default:
                    synErr::processError(ctx,L"Unknown expr_wrap type ",__FILE__,__func__,__LINE__);
            }

            struct offset element;
            element.type=type;
            element.offset=offset;

            stackOffsets.emplace_back(element);
        }

        ctx.Asm->writeRegister(1,dest);// init dest
        for(auto a: stackOffsets){
            ctx.Asm->pop(a.offset,14);
            switch(a.type){
            case offset::MUL:
                ctx.stream<<"# r"<<dest<<" = r"<<dest<<" * r14"<<std::endl;// TODO: dest = dest*r14
                break;
            case offset::DIV:
                ctx.stream<<"# r"<<dest<<" = r"<<dest<<" / r14"<<std::endl;// TODO: dest = dest/r14
                break;
            case offset::MOD:
                ctx.stream<<"# r"<<dest<<" = r"<<dest<<" % r14"<<std::endl;// TODO: dest = dest%r14
                break;
            }
        }
    }

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
        Ptr(ctx,obj.ptr,dest);
        break;
    case power::VAR:
        ctx.Asm->pop(ctx.variables[util::wstr2str(obj.var)].offset,dest);
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
    switch (obj.base->type)
    {
    case value::IMM:
        ctx.Asm->peek_i(obj.base->imm,offset,dest);
        break;
    case value::IDENT:
        key = util::wstr2str(obj.base->ident);
        if(ctx.variables.count(key)==0){
            // doesn't have key
            synErr::processError(ctx,obj.base->ident+L" is not found!",__FILE__,__func__,__LINE__);
        }
        ctx.Asm->pop(ctx.variables[key].offset,14);
        ctx.Asm->peek(offset,dest,14);
        break;
    case value::PTR:
        Ptr(ctx,obj.base->pointer,14);
        ctx.Asm->peek(offset,dest,14);
    default:
        break;
    }
    ctx.Asm->stack_offset=offs;
}