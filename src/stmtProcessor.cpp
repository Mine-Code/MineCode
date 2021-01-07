#include <stmtProcessor.h>
#include <parserTypes.h>
#include <parserCore.h>
#include <util.h>
#include <syntaxError.h>

using namespace synErr;

void stmtProcessor::For    (
    Context& ctx,
    std::wstring target,
    std::wstring iter
){
    std::wcout<<"for iterator "<<target<<" in "<<iter<<std::endl;
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
}

void stmtProcessor::Forr   (Context& ctx,int start,int end){
    ctx.Asm->startOfLoop(end-start,start);
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
    ctx.Asm->endOfLoop();
}

void stmtProcessor::While  (Context& ctx){
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
}

void stmtProcessor::If     (Context& ctx){
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
}

void stmtProcessor::Func   (Context& ctx){
    ctx.Asm->startOfFunction();
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
    ctx.Asm->endOfFunction();
}

void stmtProcessor::Put    (Context& ctx){

}

void stmtProcessor::Assign (Context& ctx,std::wstring target,std::wstring op,struct expr& value){
    // check: is avail variable of target
    if(ctx.variables.count(util::wstr2str(target))==0){
        processError(ctx,target+L" is not found",__FILE__,__func__,__LINE__);
    }
    ctx.Asm->pop(ctx.variables[util::wstr2str(target)].offset);//load value
    if(op==L"++"){
        ctx.Asm->add(1);
    }
    if(op==L"--"){
        ctx.Asm->add(-1);
    }
}

void stmtProcessor::executeFunction (Context& ctx,parserTypes::ExecFunc execfunc){
    
}