#pragma once
#ifndef TOKENPARS_H
#define TOKENPARS_H

#include <string>
#include <vector>
#include <sstream>

namespace parserWrap
{
    using streamType = std::wstringstream;
    using tokenType  = std::wstring;
    using tokensType = std::vector<tokenType>;
    void program(streamType& st,tokensType tokens);
} // namespace parserWrap


#endif