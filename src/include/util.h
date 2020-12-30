#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include <iostream>

namespace util
{
    int _toInt(std::wstring);
    
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
    bool isCondOpFull(T str){
        return
               str==L"<"  || str==L">" 
            || str==L"<=" || str==L">=" 
            || str==L"==" || str==L"!="
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

    template<typename T>
    bool isAssignOp(T str){
        if(str==L"="){
            return true;
        }
        if(str==L"++" || str==L"--"){
            return true;
        }
        if(str.back() != '='){
            return false;
        }
        if(str.size()==1){
            if(str[0]!='='){
                return false;
            }
            return true;
        }else if(str.size() == 2){
            char start=str[0];
            if(isMathOp(start)||isBitOp(start)){
                return true;
            }
        }else if(str.size() == 3){
            char start=str[0];
            char second=str[1];
            if(start!=second){
                return false;
            }
            if(start=='<' || start=='>'){
                return true;
            }
        }

        return false;
    }

    template<typename T,typename T2>
    std::vector<T> convToVector(T2 str){
        return std::vector<T>(str.begin(),str.end());
    }

    template<typename T>
    bool isDec(T val){
        return inRange<T>('0',val,'9');
    }

    template<typename T>
    bool isHex(T val){
        return isDec(val)
            || inRange<T>('a',val,'f');
    }

    template<typename T>
    bool isOp(T val){
        return isBitOp(val)
            || isCondOp(val)
            || isMathOp(val);
    }

    template<typename T>
    int toInt(T _text){
        return _toInt(std::wstring(_text));
    }

    template<typename T>
    bool isimm(T text){
        int termCounter = 0;
        for(auto ch: text){
            if(ch=='+' || ch=='-'){
                termCounter+=1;
            }else if(!isdigit(ch)){
                return false;
            }
        }
        return termCounter==1;
    }

    template<typename T>
    bool isSingle(T text){
        int counter=0;
        for(auto ch: text){
            if(ch=='+' || ch=='-'){
                counter+=1;
            }
        }
        return counter==0;
    }
    
    template<typename T>
    bool isInt(T text){
        for(auto ch: text){
            if(!isdigit(ch)){
                return false;
            }
        }
        return true;
    }

    template<typename T>
    bool isFunccall(T text, T next){
        return \
            text == L"func" || 
            next == L"(";
    }

    template<typename T>
    bool isVariable(T text){
        int counter=0;
        for(auto ch: text){
            if(ch=='+' || ch=='-' || ch=='*' || ch=='/'){
                counter+=1;
            }
        }
        return counter==0;
    }
} // namespace util


#endif