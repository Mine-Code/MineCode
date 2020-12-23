#include <syntaxError.h>
#include <parserContext.h>

using namespace synErr;

void synErr::ReportError(Context ctx,errorType type){
    std::wstring strType;
    if(type==SYNTAX){
        strType=L"syntax";
    }else if(type==TOKENIZE){
        strType=L"tokenize";
    }
}