#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <typedIterator.hxx>

class parserContext{
    enum varType{
        FLOAT,
        INT
    };
    std::wstringstream stream;
    std::unordered_map<std::string,varType> variables;
};

class parser{
private:
    std::wstring string;
    int index;
    std::vector<std::wstring> tokens;
    std::wstringstream assembly;

    void error_program(iterator<wchar_t> chiter);
public:
    void tokenize();
    void parse();
    void set(std::wstring);
    void debug();
};

#endif