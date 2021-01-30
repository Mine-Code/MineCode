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

void stmtProcessor::For    (parserTypes::parserContext& ctx,std::wstring target,std::wstring iter){
    std::wcout<<"for iterator "<<target<<" in "<<iter<<std::endl;
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
}

void stmtProcessor::Forr   (parserTypes::parserContext& ctx,int start,int end){
    ctx.Asm->startOfLoop(end-start,start);
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
    ctx.Asm->endOfLoop();
}

void stmtProcessor::While  (parserTypes::parserContext& ctx,cond conditional){
    int id=ctx.Asm->whileBegin();
    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
    std::wstring outerWhile = ctx.Asm->getEnd_While(id);
    condeval::Cond(ctx,conditional,L"",outerWhile);

    ctx.Asm->whileEnd(id);
}

void stmtProcessor::If     (parserTypes::parserContext& ctx, struct cond conditional){
    condeval::Cond(ctx,conditional);

    while(ctx.iter.hasData()){
        if(ctx.iter.peek()==L"}")break;
        parserCore::stmt(ctx);
    }
    ctx.Asm->endOfIf();
}

void stmtProcessor::Func   (parserTypes::parserContext& ctx){
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

void stmtProcessor::Put    (parserTypes::parserContext&){

}

void stmtProcessor::Assign (parserTypes::parserContext& ctx,value _target,std::wstring op,struct expr& val){
    if(_target.type==value::IDENT){
        std::string target=util::wstr2str(_target.ident);
        // check: is avail variable of target
        if(ctx.variables.count(target)==0){
            // check: is [op==equal and not have element]
            if(op==L"="){
                // make variable
                varType var;

                ctx.Asm->stack_offset+=4;
                var.offset=ctx.Asm->stack_size - ctx.Asm->stack_offset + 4;

                ctx.variables[target]=var;
            }else{
                processError(ctx,_target.ident+L" is not found",__FILE__,__func__,__LINE__);
            }
        }else {
            ctx.Asm->pop(ctx.variables[target].offset);//load value
        }
    }else if(_target.type==value::PTR){
        switch(_target.pointer.base->type){
        case value::PTR:
            eval::Ptr(ctx,_target.pointer.base->pointer,13);
            break;
        case value::IDENT:
            eval::Var(ctx,_target.pointer.base->ident,13);
            break;
        case value::STR:
            processError(ctx,L"can not set string to pointer address",__FILE__,__func__,__LINE__);
            break;
        case value::IMM:
            ctx.Asm->writeRegister(_target.pointer.base->imm,13);
            break;
        default:
           processError(ctx,L"unknown pointer base type: "+std::to_wstring(_target.type),__FILE__,__func__,__LINE__);
        }
        // calculate all offsets
        int offset;
        for (auto off: _target.pointer.offsets)
        {
            offset+=off;
        }
        ctx.Asm->add(offset,13,13);
    }else{
        processError(ctx,std::to_wstring(_target.type)+L" is not implemented...",__FILE__,__func__,__LINE__);
    }
    // Load value
    eval::Expr(ctx,val,14);

    if(op==L"++"){
        ctx.Asm->add(1,14,14);
    }else if(op==L"--"){
        ctx.Asm->add(-1,14,14);
    }else if(op==L"="){
        
    }else if( (  util::isMathOp(op[0]) || util::isBitOpFull(op.substr(0,op.length()-1))  ) && op[op.length()-1]==L'='){
        op=op.substr(0,op.length()-1);
        eval::Expr(ctx,val,14);
        if(op==L"+"){
            ctx.Asm->addReg(14);
        }else if(op==L"-"){
            ctx.Asm->subReg(14);
        }else if(op==L"/"){
            ctx.Asm->divReg(14);
        }else if(op==L"*"){
            ctx.Asm->mulReg(14);
        }else if(op==L"%"){
            ctx.Asm->modReg(14);
        }else if(op==L"<<"){
            ctx.Asm->shtlReg(14);
        }else if(op==L">>"){
            ctx.Asm->shtrReg(14);
        }else{
            processError(ctx,L"Unknown operator type: "+op,__FILE__,__func__,__LINE__);
        }

    }
    if(_target.type==value::IDENT){
        std::string target=util::wstr2str(_target.ident);
        // check: is avail variable of target
        ctx.Asm->poke(ctx.variables[target].offset,1,13);
    }else if(_target.type==value::PTR){
        std::string target=util::wstr2str(_target.ident);
        // check: is avail variable of target
        ctx.Asm->poke(ctx.variables[target].offset,1,13);
    }else{
        processError(ctx,_target.type+L" is not implemented...",__FILE__,__func__,__LINE__);
    }
}

void stmtProcessor::executeFunction (parserTypes::parserContext& ctx,ExecFunc call){
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