#pragma once
#ifndef ASM_H
#define ASM_H

#include <sstream>

class Assembly {
private:
    int stack_size = 76;
public:
    void startOfFunction(std::wstringstream& ss);
    void endOfFunction(std::wstringstream& ss);
    void callFunction(std::wstringstream& ss, int address);
};

#endif