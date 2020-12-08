#include <parser.h>
#include <iostream>
#include <chariterator.h>

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
    //TODO: tokenize for word,oct,dec by chariterator
    chariterator chiter(string);
    wchar_t ch;
    while(chiter.hasData()){
        ch=chiter.next();
        if(ch=='0'){
            wchar_t type=chiter.peek();
            std::wstring value;
            switch (type)
            {
            case 'x':
                //hex
                ch=chiter.next(); // skip '0'
                ch=chiter.next(); // skip 'x'
                while(1){
                    ch=chiter.peek();
                    if(!chiter.hasData()){
                        std::wcerr
                            <<"Invalid Program!!!"<<std::endl
                            <<chiter.index<<std::endl
                            <<string.substr(chiter.index-10,20)<<std::endl
                        ;
                            
                        throw "invalid program";
                    }
                    else if(
                           ('0'<=ch&&ch<='9')
                        || ('A'<=ch&&ch<='F')
                        || ('a'<=ch&&ch<='f')
                    ){
                        value+=chiter.next();
                    }else{
                        break;
                    }
                }
                std::wcout<<"0x"<<value<<std::endl;
                break;
            
            default:
                std::wcout<<type<<std::endl;
                break;
            }
        }
    }
}