#include <syntaxError.h>
#include <parserTypes.h>

using namespace synErr;

void synErr::ReportError(Context ctx,errorType type,std::wstring exMsg,const char* fname,const char* funcname,int line){
    std::wstring strType;
    if(type==SYNTAX){
        strType=L"syntax";
    }else if(type==TOKENIZE){
        strType=L"tokenize";
    }else if(type==PROCESS){
        strType=L"process";
    }

    std::wcout
        << "-------Error--------"               <<std::endl
        << "| type:"<<strType                   <<std::endl;
    if(exMsg!=L"")std::wcout
        << "| msg :"<<exMsg                     <<std::endl;
    std::wcout
        << "| Where?"                           <<std::endl
        << "|  func: "<<funcname                <<std::endl
        << "|  copyable: "<<fname<<":"<<line    <<std::endl
        << "| Debug Informations"               <<std::endl
        << "|  peek(-1):"<<ctx.iter.peekSafe(-1)<<std::endl
        << "|  peek( 0):"<<ctx.iter.peekSafe( 0)<<std::endl
        << "|  peek(+1):"<<ctx.iter.peekSafe(+1)<<std::endl
        << "--------------------"               <<std::endl;
    
    throw exMsg;
}
void synErr::syntaxError  (Context ctx,std::wstring ex,const char* fname,const char* funcname,int line) {ReportError(ctx,SYNTAX,ex,fname,funcname,line);}
void synErr::tokenizeError(Context ctx,std::wstring ex,const char* fname,const char* funcname,int line) {ReportError(ctx,TOKENIZE,ex,fname,funcname,line);}
void synErr::processError (Context ctx,std::wstring ex,const char* fname,const char* funcname,int line) {ReportError(ctx,PROCESS,ex,fname,funcname,line);}