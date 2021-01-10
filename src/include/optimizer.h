#pragma once
#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#include <vector>
#include <string>

class Iasm{
public:
    std::wstring name;
};

class push: public Iasm{
    int offs;
    int dest;
};

class optimizer{
private:
    std::vector<std::wstring> codes;
public:
    optimizer(std::wstring src);
    std::vector<std::wstring> get();
};

#endif