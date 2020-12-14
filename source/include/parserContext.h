#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <unordered_map>
#include <sstream>
#include "typedIterator.hxx"

class parserContext{
    enum varType{
        FLOAT,
        INT
    };
    std::wstringstream stream;
    iterator<std::wstring> iter;
    std::unordered_map<std::string,varType> variables;
};

#endif