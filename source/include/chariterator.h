#pragma once
#ifndef CHITER_H
#define CHITER_H

#include <string>
#include <vector>

class chariterator
{
private:
    std::wstring str;
public:
    size_t index;
    chariterator(std::wstring);
    wchar_t next();
    wchar_t prev();

    wchar_t peek();
    wchar_t peek(size_t offset);
    bool hasData();
};

#endif