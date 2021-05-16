#pragma once
#ifndef PARSER_WRAP_H
#define PARSER_WRAP_H

#include <string>
#include <sstream>
#include <vector>
#include <typedIterator.hxx>
#include <parserTypes.h>

class parserWrap
{
private:
    std::wstring string;
    std::vector<std::wstring> tokens;

    void error_program(iterator<wchar_t> chiter);

public:
    parserCore &ctx;

    void tokenize();
    std::wstring compile();
    std::wstring compile_expr();
    std::wstring compile(std::wstring source);
    std::wstring compile_expr(std::wstring source);

    std::wstring compile_full(std::wstring source);
    void set(std::wstring);
    void debug();
    void reset();
    void clear();
};

#endif