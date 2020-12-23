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
        TOKENIZE
    };
    using Context=parserCtx::parserContext;
    void ReportError(Context,errorType);
} // namespace synErr


#endif