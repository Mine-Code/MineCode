#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <sstream>
#include <vector>

class chariterator;

class parser{
private:
    std::wstring string;
    int index;
    std::vector<std::wstring> tokens;
    std::wstringstream assembly;

    void error_program(chariterator chiter);
public:
    void tokenize();
    void parse();
    void set(std::wstring);
    void debug();
};

#endif