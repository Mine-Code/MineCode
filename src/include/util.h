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
        //preprocessing _text
        T text;
        int sign;
        if(_text[0]=='+' || _text[0]=='-'){
            text=_text.substr(1);
            sign=+1;
        }else{
            text=_text;
            sign=-1;
        }
        int value=0;
        if(text[0]==L'0' && (text[1]==L'x' || text[1]==L'o') ){
            if(text[1]==L'x'){
                for(auto ch: text.substr(2)){
                    value*=0x10;
                    if(util::inRange(L'0',ch,L'9')){
                        value+=ch-'0';
                    }else{
                        value+=tolower(ch)-'a';
                    }
                }
            }else if(text[1]==L'o'){
                for(auto ch: text.substr(2)){
                    value*=8;
                    value+=ch-'0';
                }
            }
        }else{
            for(auto ch: text){
                value*=10;
                value+=ch-'0';
            }
        }
        return value*sign;
    }
} // namespace util


#endif