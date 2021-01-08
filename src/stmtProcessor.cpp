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

void stmtProcessor::Assign (Context& ctx,std::wstring _target,std::wstring op,struct parserTypes::expr& val){
    std::string target=util::wstr2str(_target);
    // check: is avail variable of target
    if(ctx.variables.count(target)==0){
        // check: is [op==equal and not have element]
        if(op==L"="){
            // make variable
            parserTypes::varType var;
            eval::Expr(ctx,val,13);

            var.offset=ctx.Asm->push();

            ctx.variables[target]=var;
        }else{
            processError(ctx,_target+L" is not found",__FILE__,__func__,__LINE__);
        }
    }
    ctx.Asm->pop(ctx.variables[target].offset);//load value
    if(op==L"++"){
        ctx.Asm->add(1);
    }else if(op==L"--"){
        ctx.Asm->add(-1);
    }else{
        // TODO:two value assign
    }
}

void stmtProcessor::executeFunction (Context& ctx,parserTypes::ExecFunc call){
    if(call.type==parserTypes::ExecFunc::ADDRESS){
        // address based
    }else if(call.type==parserTypes::ExecFunc::Name){
        // name based
    }
}