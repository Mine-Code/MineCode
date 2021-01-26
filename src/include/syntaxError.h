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
    void ReportError(parserTypes::parserContext&,errorType,std::wstring,const char*,const char*,int);

    void   syntaxError(parserTypes::parserContext&,std::wstring,const char*,const char*,int);
    void tokenizeError(parserTypes::parserContext&,std::wstring,const char*,const char*,int);
    void  processError(parserTypes::parserContext&,std::wstring,const char*,const char*,int);
} // namespace synErr


#endif