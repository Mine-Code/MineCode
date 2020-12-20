#include <stmtProcessor.h>
#include <parserContext.h>
#include <parserCore.h>

void stmtProcessor::For    (
    parserCtx::parserContext& ctx,
    std::wstring target,
    std::wstring iter
){
    std::wcout<<target<<iter<<std::endl;
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
}

void stmtProcessor::Forr   (parserCtx::parserContext& ctx){

}

void stmtProcessor::While  (parserCtx::parserContext& ctx){

}

void stmtProcessor::If     (parserCtx::parserContext& ctx){

}

void stmtProcessor::Func   (parserCtx::parserContext& ctx){
    ctx.Asm->startOfFunction();
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
    ctx.Asm->endOfFunction();
}

void stmtProcessor::Put    (parserCtx::parserContext& ctx){

}

void stmtProcessor::Assign (parserCtx::parserContext& ctx){

}
