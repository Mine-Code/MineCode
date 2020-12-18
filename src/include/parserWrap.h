#pragma once
#ifndef PARSER_WRAP_H
#define PARSER_WRAP_H

#include <string>
#include <sstream>
#include <vector>
#include <typedIterator.hxx>

class parserWrap{
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