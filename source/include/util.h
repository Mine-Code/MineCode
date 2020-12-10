#pragma once
#ifndef UTIL_H
#define UTIL_H

namespace util
{
    template<typename T>
    bool inRange(T min,T val,T max){
        return min<=val && val<=max;
    }

    
    template<typename T>
    bool isMathOp(T ch){
        return
            || ch=='+'|| ch=='-'
            || ch=='*'|| ch=='/'
            || ch=='%'
        ;
    }

    template<typename T>
    bool isIdentity(T ch){
        return
               ch=='{'|| ch=='}'
            || ch=='('|| ch==')'
            || ch=='['|| ch==']'

            || ch=='+'|| ch=='-'
            || ch=='*'|| ch=='/'
            || ch=='%'
            
            || ch=='|'|| ch=='^'
            || ch=='&'

            || ch=='<'|| ch=='>'

            || ch=='='
            
            || ch==';'
            || ch=='.'
        ;
    }
} // namespace util


#endif