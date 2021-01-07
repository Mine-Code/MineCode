#include <stmtProcessor.h>
#include <parserTypes.h>
#include <parserCore.h>
#include <util.h>
#include <syntaxError.h>
#include <eval.h>

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

void stmtProcessor::Put    (Context&){

}

void stmtProcessor::Assign (Context& ctx,std::wstring target,std::wstring op,struct parserTypes::expr& val){
    // check: is avail variable of target
    if(ctx.variables.count(util::wstr2str(target))==0){
        // check: is [op==equal and not have element]
        if(op==L"="){
            // make variable
            parserTypes::varType var;
            eval::Expr(val,13);

            var.offset=ctx.Asm->push();

            ctx.variables[util::wstr2str(target)]=var;
        }else{
            processError(ctx,target+L" is not found",__FILE__,__func__,__LINE__);
        }
    }
    ctx.Asm->pop(ctx.variables[util::wstr2str(target)].offset);//load value
    if(op==L"++"){
        ctx.Asm->add(1);
    }
    if(op==L"--"){
        ctx.Asm->add(-1);
    }
}

void stmtProcessor::executeFunction (Context&,parserTypes::ExecFunc){
    
}