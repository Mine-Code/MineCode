#pragma once
#ifndef TOKENPARS_H
#define TOKENPARS_H

#include <string>
#include <vector>
#include <sstream>

#include "typedIterator.hxx"
namespace parserWrap
{
    using streamType = std::wstringstream;
    using tokenType  = std::wstring;
    using tokensType = iterator<std::wstring>;
    void program(streamType& st,tokensType tokens);
} // namespace parserWrap


#endif