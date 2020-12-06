#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

class parser{
private:
    std::wstring string;
    int index;
    std::vector<std::wstring> tokens;
public:
    void tokenize();
    void set(std::wstring);
    void debug();
};

#endif