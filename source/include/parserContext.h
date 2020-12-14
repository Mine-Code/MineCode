#pragma once
#ifndef PARSCTX_H
#define PARSCTX_H

#include <unordered_map>
#include <sstream>
#include "typedIterator.hxx"

namespace parserCtx
{
    using streamType = std::wstringstream;
    using iterType   = iterator<std::wstring>;
    using varsType   = std::unordered_map<std::string,varType>;
    enum varType{
        FLOAT,
        INT
    };
    struct parserContext{
        parserContext();
        streamType stream;
        iterType iter;
        varsType variables;
    };

} // namespace parserCtx


#endif