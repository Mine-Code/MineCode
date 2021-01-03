#pragma once
#ifndef MCL_H
#define MCL_H

#include <string>
#include <../../lib/json/single_include/nlohmann/json.hpp>

namespace parserTypes
{
    class parserContext;
} // namespace parserTypes

class mcl{
public:
    json raw;
    mcl(std::string name);
};

void operator<<(parserTypes::parserContext&, mcl);

#endif