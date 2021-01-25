#include <stmtProcessor.h>
#include <parserTypes.h>
#include <parserCore.h>
#include <util.h>
#include <syntaxError.h>
#include <eval.h>
#include <condeval.h>
#include <condutil.h>

using namespace synErr;
using namespace parserTypes;

void stmtProcessor::For    (Context& ctx,std::wstring target,std::wstring iter){
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

void stmtProcessor::While  (Context& ctx,cond conditional){
    int id=ctx.Asm->whileBegin();
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
    std::wstring outerWhile = ctx.Asm->getEnd_While(id);
    condeval::Cond(ctx,conditional,L"",outerWhile);

    ctx.Asm->whileEnd(id);
}

void stmtProcessor::If     (Context& ctx, struct cond conditional){
    condeval::Cond(ctx,conditional);

    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
    ctx.Asm->endOfIf();
}

void stmtProcessor::Func   (Context& ctx){
    ctx.Asm->startOfFunction();
    
    int r14 = ctx.Asm->push(14);
    int r15 = ctx.Asm->push(15);

    ctx.stream<<"# Inner Function\n";
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
    ctx.stream<<"# Outer Function\n";

    ctx.Asm->pop(r14,14);
    ctx.Asm->pop(r15,15);

    ctx.Asm->endOfFunction();
}

void stmtProcessor::Put    (Context&){

}

void stmtProcessor::Assign (Context& ctx,value _target,std::wstring op,struct expr& val){
    if(_target.type==value::IDENT){
        std::string target=util::wstr2str(_target.ident);
        // check: is avail variable of target
        if(ctx.variables.count(target)==0){
            // check: is [op==equal and not have element]
            if(op==L"="){
                // make variable
                varType var;
                eval::Expr(ctx,val,13);

                var.offset=ctx.Asm->push();

                ctx.variables[target]=var;
            }else{
                processError(ctx,_target.ident+L" is not found",__FILE__,__func__,__LINE__);
            }
        }else {
            ctx.Asm->pop(ctx.variables[target].offset);//load value
        }
    }else{
        processError(ctx,_target.type+L" is not implemented...",__FILE__,__func__,__LINE__);
    }
    if(op==L"++"){
        ctx.Asm->add(1);
    }else if(op==L"--"){
        ctx.Asm->add(-1);
    }else if(op==L"="){
        eval::Expr(ctx,val,14);
        std::string name;

        switch (_target.type)
        {
        case value::IDENT:
            name=util::wstr2str(_target.ident);
            if(ctx.variables[name].type == varType::INT){
                ctx.Asm->poke(ctx.variables[name].offset,1,14);
            }else if(ctx.variables[name].type == varType::FLOAT){
                processError(ctx,L"assign to "+std::to_wstring(_target.type)+L"in float is not implemented...",__FILE__,__func__,__LINE__);
            }
            break;
        
        default:
            processError(ctx,L"assign to "+std::to_wstring(_target.type)+L" is not implemented...",__FILE__,__func__,__LINE__);
            break;
        }
    }else{
        op=op.substr(0,op.length()-1);
        // TODO: process two value assign using op

    }
}

void stmtProcessor::executeFunction (Context& ctx,ExecFunc call){
    //load arguments
    int n=3;
    for(auto arg: call.args){
        eval::Expr(ctx,arg,n++);
    }
    //load address
    if(call.type==ExecFunc::ADDRESS){
        // address based
        ctx.Asm->writeRegister(call.funcAddr,15);
    }else if(call.type==ExecFunc::Name){
        // name based
        ctx.Asm->pop(ctx.variables[util::wstr2str(call.funcId)].offset,15);
    }
    ctx.stream<<"mtctr r15\n"
                "btcrl\n";
}