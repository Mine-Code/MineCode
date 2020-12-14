#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <vector>
namespace util
{
    template<typename T>
    bool inRange(T min,T val,T max){
        return min<=val && val<=max;
    }

    
    template<typename T>
    bool isMathOp(T ch){
        return
               ch=='+'|| ch=='-'
            || ch=='*'|| ch=='/'
            || ch=='%'
        ;
    }

    template<typename T>
    bool isBitOp(T ch){
        return
               ch=='|'|| ch=='^'
            || ch=='&'
        ;
    }

    template<typename T>
    bool isCondOp(T ch){
        return
               ch=='<'|| ch=='>'
            || ch=='='|| ch=='!'
        ;
    }

    template<typename T>
    bool isIdentity(T ch){
        return
               ch=='{'|| ch=='}'
            || ch=='('|| ch==')'
            || ch=='['|| ch==']'

            || isMathOp(ch)
            
            || isBitOp(ch)

            || isCondOp(ch)
            
            || ch==';'
            || ch=='.'
        ;
    }

    template<typename T,typename T2>
    std::vector<T> convToVector(T2 str){
        return std::vector<T>(str.begin(),str.end());
    }
} // namespace util


#endif