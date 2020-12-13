#include <parser.h>
#include <iostream>
#include <chariterator.h>
#include <util.h>
#include <parserWrap.h>

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
void parser::error_program(chariterator chiter){
    std::wcerr
        <<"Invalid Program!!!"<<std::endl
        <<chiter.index<<std::endl
        <<string.substr(chiter.index-10,20)<<std::endl
    ;
                            
    throw "invalid program";
}
void parser::tokenize(){
    chariterator chiter(string);
    wchar_t ch;
    std::ios::fmtflags bk = std::wcout.flags();
    while(chiter.hasData()){
        ch=chiter.next();
        if(ch=='0'){
            wchar_t type=chiter.peek();
            std::wstring value;
            switch (type)
            {
            case 'x': //hex
                value+=L"0x";
                chiter.next(); // skip 'x'
                while(1){
                    ch=std::tolower(chiter.peek());
                    if(!chiter.hasData()){
                        error_program(chiter);
                    }
                    else if(
                           util::inRange<char>('0',ch,'9')
                        || util::inRange<char>('a',ch,'f')
                    ){
                        auto tmp=std::tolower(chiter.next());
                        value+=tmp;
                    }else{
                        break;
                    }
                }
                tokens.emplace_back(value);
                break;
            
            case 'o': //oct
                value+=L"0o";
                chiter.next(); // skip 'o'
                while(1){
                    ch=chiter.peek();
                    if(!chiter.hasData()){
                        error_program(chiter);
                    }
                    else if(util::inRange<char>('0',ch,'8')){
                        value+=chiter.next();
                    }else{
                        break;
                    }
                }
                tokens.emplace_back(value);
                break;

            default:
                error_program(chiter);
                break;
            }
        }
        else if(isdigit(ch)){
            std::wstring value;
            value+=ch;
            while(1){
                ch=chiter.peek();
                if(!chiter.hasData()){
                    error_program(chiter);
                }
                else if(util::inRange<char>('0',ch,'9')){
                    ch+=chiter.next();
                }else{
                    break;
                }
            }
            tokens.emplace_back(value);
        }else if(ch=='f' && chiter.hasData() && chiter.peek() =='\"'){
            std::wstring value;
            value+=ch;
            value+=chiter.next();
            while(1){
                ch=chiter.next();
                if(!chiter.hasData()){
                    error_program(chiter);
                }else if(ch=='\\'){
                    value+=ch;
                    ch=chiter.next();
                    if(!chiter.hasData()){
                        error_program(chiter);
                    }
                    value+=ch;
                    continue;
                }
                value+=ch;
                if(ch==L'"'){
                    break;
                }
            }
            tokens.emplace_back(value);
        }else if((ch=='+'||ch=='-') && chiter.hasData() && chiter.peek() ==ch){
            std::wstring value;
            value+=ch;
            value+=ch;
            tokens.emplace_back(value);
            chiter.next();
        }else if(iswalpha(ch)){
            std::wstring value;
            value+=ch;
            while(1){
                ch=chiter.peek();
                if(!chiter.hasData()){
                    error_program(chiter);
                }
                else if(iswalpha(ch)){
                    value+=chiter.next();
                }else{
                    break;
                }
            }
            tokens.emplace_back(value);
        }else if(ch == L'\"'){
            std::wstring value;
            value+=ch;
            while(1){
                ch=chiter.next();
                if(!chiter.hasData()){
                    error_program(chiter);
                }else if(ch=='\\'){
                    value+=ch;
                    ch=chiter.next();
                    if(!chiter.hasData()){
                        error_program(chiter);
                    }
                    value+=ch;
                    continue;
                }
                value+=ch;
                if(ch==L'"'){
                    break;
                }
            }
            tokens.emplace_back(value);
        }else if(util::isIdentity(ch)){
            std::wstring tmp;
            tmp+=ch;
            if(
                  util::isBitOp(ch)
                ||util::isCondOp(ch)
                ||util::isMathOp(ch)
            ){
                if(chiter.peek(1)=='='){
                    tmp+=chiter.next();
                }
            }
            if(ch=='<' || ch=='>'){
                if(chiter.peek()==ch){
                    tmp+=ch;
                    chiter.next();
                }
            }
            tokens.emplace_back(tmp);
        }else{
            //std::wcout<<ch<<std::endl;
        }
    }
    std::wcout.flags(bk);
}

void parser::parse(){
    std::wstringstream st;
    parserWrap::program(st,tokens);
    // process 'st.str()'
}