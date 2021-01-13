#include <condeval.h>

#include <parserTypes.h>
#include <eval.h>

using namespace parserTypes;

void condeval::Cond(parserContext ctx, cond cond){
    CondChild(ctx,cond.first);
    // TODO: process cond.conds
}
void condeval::CondChild(parserContext ctx, condChild cond){
    if(cond.op == condChild::single){
        // TODO: process cond.single
    }else{
        // TODO: process cond.op and val1/2
    }
}