#pragma once
#ifndef ASM_H
#define ASM_H

#include <sstream>

class Assembly {
public:
    int stack_size = 76;
    std::wstringstream& ss;

    Assembly(std::wstringstream& ss);
    void startOfFunction();
    void endOfFunction();
    void callFunction(int address);
    void peek(int address, int dest = 13);
    void poke(int address, int src = 13);
    void add(int value, int src = 13, int dest = 13);
    void mul(int value, int src = 13, int dest = 13);
    void div(int value, int src = 13, int dest = 13);
};

#endif