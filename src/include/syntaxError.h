#pragma once
#ifndef SYNERR_H
#define SYNERR_H

#include <iostream>

namespace parserCtx
{
    class parserContext;
} // namespace parserCtx

namespace synErr
{
    enum errorType{
        SYNTAX,
        TOKENIZE,
        PROCESS
    };
    using Context=parserCtx::parserContext&;
    void ReportError(Context,errorType,std::wstring);

    void syntaxError(Context,errorType,std::wstring);
    void tokenizeError(Context,errorType,std::wstring);
    void processError(Context,errorType,std::wstring);
} // namespace synErr


#endif