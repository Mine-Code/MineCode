#pragma once
#ifndef SYNERR_H
#define SYNERR_H

#include <iostream>

class parserCore;
namespace synErr
{
    enum errorType
    {
        SYNTAX,
        TOKENIZE,
        PROCESS
    };
    void ReportError(parserCore *, errorType, std::wstring, const char *, const char *, int);

    void syntaxError(parserCore *, std::wstring, const char *, const char *, int);
    void tokenizeError(parserCore *, std::wstring, const char *, const char *, int);
    void processError(parserCore *, std::wstring, const char *, const char *, int);
} // namespace synErr

#endif