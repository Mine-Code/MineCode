#include <condeval.h>

#include <parserTypes.h>
#include <eval.h>
#include <util.h>

using namespace parserTypes;

void condeval::Cond(parserContext ctx, cond cond){
    CondChild(ctx,cond.first);
    // TODO: process cond.conds
}
void condeval::CondChild(parserContext ctx, condChild cond){
    if(cond.op == condChild::SINGLE){
        varType var;
        switch(cond.single.type){
        case value::IDENT:
            var = ctx.variables[util::wstr2str(cond.single.ident)];
            if(var.type == varType::INT){
                ctx.Asm.pop(var.offset);
                // if r13 == 1
            }else{
                // TODO: show error message
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