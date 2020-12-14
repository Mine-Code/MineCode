#pragma once
#ifndef PARSCTX_H
#define PARSCTX_H

#include <unordered_map>
#include <sstream>
#include "typedIterator.hxx"

namespace parserCtx
{
    enum varType{
        FLOAT,
        INT
    };
    struct parserContext{
        parserContext();
        std::wstringstream stream;
        iterator<std::wstring> iter;
        std::unordered_map<std::string,varType> variables;
    };
    
} // namespace parserCtx


#endif