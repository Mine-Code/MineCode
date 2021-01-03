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

namespace nlohmann
{
    template<typename T = void, typename SFINAE = void>
    struct adl_serializer;

    template<template<typename U, typename V, typename... Args> class ObjectType =
             std::map,
             template<typename U, typename... Args> class ArrayType = std::vector,
             class StringType = std::string, class BooleanType = bool,
             class NumberIntegerType = std::int64_t,
             class NumberUnsignedType = std::uint64_t,
             class NumberFloatType = double,
             template<typename U> class AllocatorType = std::allocator,
             template<typename T, typename SFINAE = void> class JSONSerializer =
             adl_serializer,
             class BinaryType = std::vector<std::uint8_t>>
    class basic_json;
} // namespace nlohmann


class mcl{
    using json=nlohmann::basic_json<>;
public:
    json raw;
    mcl(std::string name);
};

void operator<<(parserTypes::parserContext&, mcl);

#endif