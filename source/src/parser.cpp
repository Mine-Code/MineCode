#include <parser.h>
#include <iostream>

void parser::debug(){
    for(auto token:tokens){
        std::cout<<token.c_str()<<std::endl;
    }
}

void parser::set(std::wstring src){
    string=src;
}