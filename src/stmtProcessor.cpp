#include <stmtProcessor.h>
#include <parserContext.h>
#include <parserCore.h>

void For    (parserCtx::parserContext& ctx){
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
}

void Forr   (parserCtx::parserContext& ctx){

}

void While  (parserCtx::parserContext& ctx){

}

void If     (parserCtx::parserContext& ctx){

}

void Func   (parserCtx::parserContext& ctx){

}

void Put    (parserCtx::parserContext& ctx){

}

void Assign (parserCtx::parserContext& ctx){

}
