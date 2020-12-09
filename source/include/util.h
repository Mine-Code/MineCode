#pragma once
#ifndef UTIL_H
#define UTIL_H

namespace util
{
    template<typename T>
    bool inRange(T min,T val,T max){
        return min<=val && val<=max;
    }
} // namespace util


#endif