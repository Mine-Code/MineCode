#pragma once
#ifndef SYNERR_H
#define SYNERR_H

#include <iostream>

namespace parserTypes
{
    class parserContext;
} // namespace parserTypes

namespace synErr
{
    enum errorType{
        SYNTAX,
        TOKENIZE,
        PROCESS
    };
    using Context=parserTypes::parserContext&;
    void ReportError(Context,errorType,std::wstring,const char*,const char*,int);

    void   syntaxError(Context,std::wstring,const char*,const char*,int);
    void tokenizeError(Context,std::wstring,const char*,const char*,int);
    void  processError(Context,std::wstring,const char*,const char*,int);
} // namespace synErr


#endif