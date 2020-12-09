#include <parser.h>
#include <iostream>
#include <chariterator.h>

void parser::debug(){
    std::wcout<<"source"<<std::endl;
    std::wcout<<"--------------------"<<std::endl;
    std::wcout<<string<<std::endl;
    std::wcout<<"--------------------"<<std::endl;
    std::wcout<<"tokens"<<std::endl;
    for(auto token:tokens){
        std::wcout<<"  "<<token.c_str()<<std::endl;
    }
}

void parser::set(std::wstring src){
    string=src;
}
void parser::tokenize(){
    chariterator chiter(string);
    wchar_t ch;
    std::ios::fmtflags bk = std::wcout.flags();
    while(chiter.hasData()){
        ch=chiter.next();
        if(ch=='0'){
            wchar_t type=chiter.peek();
            uint64_t value=0;
            switch (type)
            {
            case 'x': //hex
                chiter.next(); // skip 'x'
                while(1){
                    ch=std::tolower(chiter.peek());
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
                        || ('a'<=ch&&ch<='f')
                    ){
                        auto tmp=std::tolower(chiter.next());
                        value<<=4;
                        if('0'<=ch&&ch<='9'){
                            value+=(int)(tmp-'0');
                        }else if('a'<=ch&&ch<='f'){
                            value+=(int)(tmp-'a'+10);
                        }
                    }else{
                        break;
                    }
                }
                std::wcout<<"0x"<<std::hex<<value<<std::endl;
                break;
            
            case 'o': //oct
                chiter.next(); // skip 'o'
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
                    else if(('0'<=ch&&ch<='8')){
                        value<<=3;
                        value+=chiter.next()-'0';
                    }else{
                        break;
                    }
                }
                std::wcout<<"0o"<<std::oct<<value<<std::endl;
                break;

            default:
                std::wcerr
                    <<"Invalid Program!!!"<<std::endl
                    <<chiter.index<<std::endl
                    <<string.substr(chiter.index-10,20)<<std::endl
                ;
                    
                throw "invalid program";
                break;
            }
        }
        else if(isdigit(ch)){
            uint64_t value=(int)(ch-'0');
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
                else if('0'<=ch&&ch<='9'){
                    value*=10;
                    value+=(int)(chiter.next()-'0');
                }else{
                    break;
                }
            }
            std::wcout<<"0d"<<std::dec<<value<<std::endl;
        }else{
        }
    }
    std::wcout.flags(bk);
}