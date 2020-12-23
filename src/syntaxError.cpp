#include <syntaxError.h>
#include <parserContext.h>

using namespace synErr;

void synErr::ReportError(Context ctx,errorType type,std::wstring exMsg){
    std::wstring strType;
    if(type==SYNTAX){
        strType=L"syntax";
    }else if(type==TOKENIZE){
        strType=L"tokenize";
    }

    std::wcout
        << "-------Error--------"<<std::endl
        << "| type:"<<strType<<std::endl;
    if(exMsg!=L"")std::wcout
        << "|  msg:"<<exMsg<<std::endl;
    std::wcout
        << "| Debug Informations"
        << "|  peek(-1):"<<ctx.iter.peek(-1)<<std::endl
        << "|  peek( 0):"<<ctx.iter.peek( 0)<<std::endl
        << "|  peek(+1):"<<ctx.iter.peek(+1)<<std::endl
        << "--------------------"<<std::endl;
    
}