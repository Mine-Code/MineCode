#include "util.h"
#include <string>

namespace util
{
    int _toInt(std::wstring _text){
        //preprocessing _text
        std::wstring text;
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
