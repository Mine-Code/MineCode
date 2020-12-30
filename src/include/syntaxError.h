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
    void ReportError(Context,errorType,std::wstring,std::wstring,std::wstring,int);

    void syntaxError(Context,std::wstring,std::wstring,std::wstring,int);
    void tokenizeError(Context,std::wstring,std::wstring,std::wstring,int);
    void processError(Context,std::wstring,std::wstring,std::wstring,int);
} // namespace synErr


#endif