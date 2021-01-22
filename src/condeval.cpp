#include <condeval.h>

#include <parserTypes.h>
#include <eval.h>
#include <util.h>
#include <syntaxError.h>
#include <condutil.h>

using namespace parserTypes;

void condeval::Cond(parserContext ctx, cond cond){
    std::wstring footer=std::to_wstring(ctx.Asm->make_if_ctr++);
    std::wstring label = L"if_"+footer;
    std::wstring endLabel = L"endif_"+footer;
    for(auto condChild: cond.conds){
        CondAnd(ctx,condChild,label);
    }
    ctx.Asm->Jump(endLabel);
    ctx.Asm->makeLabel(label);
}
void condeval::Cond(parserContext ctx, cond cond,std::wstring elseLabel){
    for(auto condChild: cond.conds){
        CondAnd(ctx,condChild,elseLabel);
    }
    ctx.Asm->Jump(elseLabel);
}
void condeval::CondAnd(parserContext ctx, condAnd cond,std::wstring target){
    if(cond.conds.size() == 1){
        CondChild(ctx,cond.conds[0],target);
    }else{
        std::wstring elseLabel=ctx.Asm->getLabel();
        for(auto child: cond.conds){
            CondChild(ctx,util::invertConditional(child),elseLabel);
        }
        ctx.Asm->Jump(target);
        ctx.Asm->makeLabel(elseLabel);
    }
}
void condeval::CondChild(parserContext ctx, condChild cond, std::wstring target){
    if(cond.op == condChild::SINGLE || cond.op == condChild::SINGLE_INV){
        varType var;
        int compareTarget=1;
        if(cond.op==condChild::SINGLE_INV){
            compareTarget=0;
        }

        switch(cond.single.type){
        case value::IDENT:
            var = ctx.variables[util::wstr2str(cond.single.ident)];
            if(var.type == varType::INT){
                ctx.Asm->pop(var.offset);
                ctx.Asm->compareImm(13,compareTarget);
                ctx.Asm->condJump(Assembly::EQU,0,target);
            }else{
                synErr::processError(ctx,L"condition variable is must be integer. but this is float",__FILE__,__func__,__LINE__);
            }
            break;
        case value::IMM:
            if(cond.single.imm==compareTarget){
                ctx.stream<<"b "<<target<<"\n";
            }
            break;
        case value::PTR:
            eval::Ptr(ctx,cond.single.pointer);
            ctx.Asm->compareImm(13,compareTarget);
            break;
        case value::STR:
            synErr::processError(ctx,L"condition value is must be integer, identity or pointer",__FILE__,__func__,__LINE__);
            break;
        }
    }else{
        // process val1/2
        eval::Expr(ctx,cond.val1,13);
        eval::Expr(ctx,cond.val2,14);
        ctx.Asm->compare(13,14);
        Assembly::condType t;
        switch (cond.op)
        {
        case condChild::EQU:  t=Assembly::EQU; break;
        case condChild::NEQ:  t=Assembly::NEQ; break;
        case condChild::LT:   t=Assembly::LT;  break;
        case condChild::GT:   t=Assembly::GT;  break;
        case condChild::GE:   t=Assembly::GE;  break;
        case condChild::LE:   t=Assembly::LE;  break;
        
        default:
            std::wcout<<"Warning: invalid conditional type id (set to equal)";
            t=Assembly::EQU;
            break;
        }
        ctx.Asm->condJump(t,0,target);
    }
}