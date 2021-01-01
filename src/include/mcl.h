#pragma once
#ifndef MCL_H
#define MCL_H

#include <string>
#include <../../lib/json/single_include/nlohmann/json.hpp>
using json = nlohmann::json;

namespace parserTypes
{
    class parserContext;
} // namespace parserTypes

class mcl{
private:
    json raw;
public:
    mcl(std::string name);
};

void operator<<(parserTypes::parserContext&, mcl);

#endif