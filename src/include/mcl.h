#pragma once
#ifndef MCL_H
#define MCL_H

#include <string>
#include <map>
#include <vector>

namespace parserTypes
{
    class parserContext;
} // namespace parserTypes

void operator<<(parserTypes::parserContext&, std::string);

#endif