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

void stmtProcessor::If     (Context& ctx, struct parserTypes::cond conditional){
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
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

void stmtProcessor::Assign (Context& ctx,parserTypes::value _target,std::wstring op,struct parserTypes::expr& val){
    if(_target.type==parserTypes::value::IDENT){
        std::string target=util::wstr2str(_target.ident);
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
                processError(ctx,_target.ident+L" is not found",__FILE__,__func__,__LINE__);
            }
        }
        ctx.Asm->pop(ctx.variables[target].offset);//load value
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
        case parserTypes::value::IDENT:
            name=util::wstr2str(_target.ident);
            if(ctx.variables[name].type == parserTypes::varType::INT){
                ctx.Asm->poke(ctx.variables[name].offset,1,14);
            }else if(ctx.variables[name].type == parserTypes::varType::FLOAT){
                processError(ctx,L"assign to "+std::to_wstring(_target.type)+L"in float is not implemented...",__FILE__,__func__,__LINE__);
            }
            break;
        
        default:
            processError(ctx,L"assign to "+std::to_wstring(_target.type)+L" is not implemented...",__FILE__,__func__,__LINE__);
            break;
        }
    }else{
        // TODO:two value assign
        std::wcout<<op<<std::endl;
    }
}

void stmtProcessor::executeFunction (Context& ctx,parserTypes::ExecFunc call){
    if(call.type==parserTypes::ExecFunc::ADDRESS){
        // address based
        ctx.Asm->writeRegister(call.funcAddr,15);
    }else if(call.type==parserTypes::ExecFunc::Name){
        // name based
        ctx.Asm->pop(ctx.variables[util::wstr2str(call.funcId)].offset,15);
    }
    //load arguments
    int n=3;
    for(auto arg: call.args){
        eval::Expr(ctx,arg,n++);
    }
    ctx.stream<<"mtctr r15\n"
                "btcrl\n";
}