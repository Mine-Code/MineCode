#include <parser.h>
#include <iostream>

void parser::debug(){
    std::cout<<"source"<<std::endl;
    std::cout<<"--------------------"<<std::endl;
    std::wcout<<string<<std::endl;
    std::cout<<"--------------------"<<std::endl;
    std::cout<<"tokens"<<std::endl;
    for(auto token:tokens){
        std::cout<<"  "<<token.c_str()<<std::endl;
    }
}

void parser::set(std::wstring src){
    string=src;
}
void parser::tokenize(){
    //TODO: tokenize for word,int
    size_t progsize=string.length();
    for (size_t i = 0; i < progsize; i++)
    {
        std::wcout<<string[i];
    }
    
}