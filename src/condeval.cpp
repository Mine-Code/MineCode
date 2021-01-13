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
        switch(cond.single.type){
        case value::IDENT:
            if(ctx.variables[util::wstr2str(cond.single.ident)].type == varType::INT){
                // TODO: implement this
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