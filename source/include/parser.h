#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

class chariterator;

class parser{
private:
    std::wstring string;
    int index;
    std::vector<std::wstring> tokens;

    void error_program(chariterator chiter);
public:
    void tokenize();
    void parse();
    void set(std::wstring);
    void debug();
};

#endif