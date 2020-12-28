#include <stmtProcessor.h>
#include <parserTypes.h>
#include <parserCore.h>

void stmtProcessor::For    (
    parserTypes::parserContext& ctx,
    std::wstring target,
    std::wstring iter
){
    std::wcout<<"for iterator "<<target<<" in "<<iter<<std::endl;
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
}

void stmtProcessor::Forr   (parserTypes::parserContext& ctx,int start,int end){
    std::wcout<<"for range "<<start<<"to"<<end<<std::endl;
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
}

void stmtProcessor::While  (parserTypes::parserContext& ctx){
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
}

void stmtProcessor::If     (parserTypes::parserContext& ctx){
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
}

void stmtProcessor::Func   (parserTypes::parserContext& ctx){
    ctx.Asm->startOfFunction();
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
    ctx.Asm->endOfFunction();
}

void stmtProcessor::Put    (parserTypes::parserContext& ctx){

}

void stmtProcessor::Assign (parserTypes::parserContext& ctx){

}
