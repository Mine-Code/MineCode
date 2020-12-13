#pragma once
#ifndef TOKENPARS_H
#define TOKENPARS_H

#include <string>
#include <vector>

namespace parserWrap
{
    typedef std::wstring tokenType;
    typedef std::vector<tokenType> tokensType;
    void program(std::wstringstream& st,tokensType tokens);
} // namespace parserWrap


#endif