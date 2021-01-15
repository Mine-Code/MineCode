#include <condeval.h>

#include <parserTypes.h>
#include <eval.h>
#include <util.h>
#include <syntaxError.h>

using namespace parserTypes;

void condeval::Cond(parserContext ctx, cond cond, int dest){
    CondChild(ctx,cond.first, 2);
    // TODO: process cond.conds
}
void condeval::CondChild(parserContext ctx, condChild cond, int dest){
    if(cond.op == condChild::SINGLE){
        varType var;
        switch(cond.single.type){
        case value::IDENT:
            var = ctx.variables[util::wstr2str(cond.single.ident)];
            if(var.type == varType::INT){
                ctx.Asm->pop(var.offset);
                // if r13 == 1
            }else{
                synErr::processError(ctx,L"condition variable is must be integer",__FILE__,__func__,__LINE__);
            }
            break;
        case value::IMM:
            // TODO: implement this
            break;
        case value::PTR:
            // TODO: implement this
            break;
        case value::STR:
            // TODO: implement this
            break;
        }
    }else{
        // TODO: process cond.op and val1/2
    }
}