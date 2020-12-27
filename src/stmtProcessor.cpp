#include <stmtProcessor.h>
#include <parserContext.h>
#include <parserCore.h>

void stmtProcessor::For    (
    parserCtx::parserContext& ctx,
    std::wstring target,
    std::wstring iter
){
    std::wcout<<"for iterator "<<target<<" in "<<iter<<std::endl;
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
}

void stmtProcessor::Forr   (parserCtx::parserContext& ctx,int start,int end){
    std::wcout<<"for range "<<start<<"to"<<end<<std::endl;
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
}

void stmtProcessor::While  (parserCtx::parserContext& ctx){
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
}

void stmtProcessor::If     (parserCtx::parserContext& ctx){
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
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
