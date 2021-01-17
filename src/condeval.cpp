#include <condeval.h>

#include <parserTypes.h>
#include <eval.h>
#include <util.h>
#include <syntaxError.h>

using namespace parserTypes;

void condeval::Cond(parserContext ctx, cond cond){
    CondChild(ctx,cond.first, L"1");
    // TODO: process cond.conds
}
void condeval::CondChild(parserContext ctx, condChild cond, std::wstring target){
    if(cond.op == condChild::SINGLE){
        varType var;
        switch(cond.single.type){
        case value::IDENT:
            var = ctx.variables[util::wstr2str(cond.single.ident)];
            if(var.type == varType::INT){
                ctx.Asm->pop(var.offset);
                ctx.Asm->compareImm(13,1);
                ctx.Asm->condJump(Assembly::EQU,0,target);
            }else{
                synErr::processError(ctx,L"condition variable is must be integer. but this is float",__FILE__,__func__,__LINE__);
            }
            break;
        case value::IMM:
            if(cond.single.imm==1){
                ctx.stream<<"b "<<target<<"\n";
            }
            break;
        case value::PTR:
            eval::Ptr(ctx,cond.single.pointer);
            ctx.Asm->compareImm(13,1);
            break;
        case value::STR:
            synErr::processError(ctx,L"condition value is must be integer, identity or pointer",__FILE__,__func__,__LINE__);
            break;
        }
    }else{
        // TODO: process cond.op and val1/2
        // process val1/2
        eval::Expr(ctx,cond.val1,13);
        eval::Expr(ctx,cond.val2,14);
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
            break;
        }
        ctx.Asm->condJump(t,0,target);
    }
}