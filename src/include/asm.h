#pragma once
#ifndef ASM_H
#define ASM_H

#include <sstream>

class Assembly {
public:
    int stack_size = 0x20;
    std::wstringstream& ss;

    Assembly(std::wstringstream& ss);
    void startOfFunction();
    void endOfFunction();

    void writeResister(int value, int dest);
    void moveResister(int src, int dest);

    void callFunction(int address);
    void peek(int address = 0, bool is_ptr = false, int offset = 0, int src = 13, int dest = 13);
    void poke(int address = 0, bool is_immediate = true, int offset = 0, int dest = 12, int src = 13);
    void add(int value, int src = 13, int dest = 13);
    void mul(int value, int src = 13, int dest = 13);
    void div(int value, int src = 13, int dest = 13);
};

#endif