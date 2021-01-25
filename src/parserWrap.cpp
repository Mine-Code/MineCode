#include <parserWrap.h>
#include <iostream>
#include <typedIterator.hxx>
#include <util.h>
#include <parserCore.h>
#include <parserTypes.h>
#include <iomanip>

void parserWrap::debug(){
    std::wcout<<"Debug"<<std::endl;
    auto backup=std::wcout.flags();
    
    // show all variables
    std::wcout<<"| Variables"<<std::endl;
    std::wcout<<"| | offset | id | name"<<std::endl;
    for(auto [key, value]: this->ctx.variables){
        std::wcout<<"| | "
            <<std::setw(6)<<value.offset<<std::setw(0)<<" | "
            <<std::setw(2)<<value.type  <<std::setw(0)<<" | "
            <<util::str2wstr(key)
        <<std::endl;
    }
    std::wcout<<"| "<<std::endl;
    
    // show all puts
    std::wcout<<"| Puts"<<std::endl;
    for(auto [key, value]: this->ctx.puts){
        std::wcout<<"| | "<<util::str2wstr(key)<<std::endl;
        std::wcout<<"| | | ";
        for(auto ch: value){
            std::wcout<<ch;
            if(ch=='\n')std::wcout<<"| | | ";
        }
        std::wcout<<"\r| |   "<<std::endl;
    }
    std::wcout<<"| "<<std::endl;

    std::wcout.flags(backup);
    std::wcout<<"-----------------"<<std::endl;
}

void parserWrap::set(std::wstring src){
    string=src;
}
void parserWrap::error_program(iterator<wchar_t> chiter){
    std::wcerr
        <<"Invalid Program!!!"<<std::endl
        <<chiter.index<<std::endl
        <<string.substr(chiter.index-10,20)<<std::endl
        <<"error on : "<<string.substr(chiter.index-1,3)<<std::endl
        <<"           "<<" ~ "<<std::endl
    ;
                            
    throw "invalid program";
}
void parserWrap::tokenize(){
    iterator<wchar_t> chiter(util::convToVector<wchar_t>(string));
    wchar_t ch,nextch;
    while(chiter.hasData()){
        ch=chiter.next();
        nextch=chiter.peekSafe();
        std::wstring value;
        if(ch=='0' && nextch == L'x'){
            value+=L"0x";
            chiter.next(); // skip 'x'
            while(chiter.hasData()){
                ch=std::tolower(chiter.peekSafe());
                if(util::isHex(ch)){
                    auto tmp=std::tolower(chiter.next());
                    value+=tmp;
                }else{
                    break;
                }
            }
            tokens.emplace_back(value);
        }else if(ch=='0' && nextch == L'o'){
            value+=L"0o";
            chiter.next(); // skip 'o'
            while(chiter.hasData()){
                ch=chiter.peekSafe();
                if(util::inRange<char>('0',ch,'8')){
                    value+=chiter.next();
                }else{
                    break;
                }
            }
            tokens.emplace_back(value);
        }else if(ch==L'.' && nextch==L'.'){
            chiter.next();
            chiter.next();
            tokens.emplace_back(L"...");
        }else if(isdigit(ch)){
            value+=ch;
            while(chiter.hasData()){
                ch=chiter.peekSafe();
                if(util::isDec(ch)){
                    value+=ch;
                    chiter.next();
                }else{
                    break;
                }
            }
            tokens.emplace_back(value);
        }else if(ch==L'=' && nextch==L'='){
            chiter.next(); //skip one equal
            tokens.emplace_back(L"==");
        }else if(ch==L'|' && nextch==L'|'){
            chiter.next(); // skip one
            tokens.emplace_back(L"||");
        }else if(ch==L'&' && nextch==L'&'){
            chiter.next(); // skip one
            tokens.emplace_back(L"&&");
        }else if(ch==L'='){
            value+=ch;
            tokens.emplace_back(value);
        }else if(util::isCondOp(ch)){
            value+=ch;
            value+=chiter.next();
            tokens.emplace_back(value);
        }else if(ch=='f' && nextch =='\"'){
            value+=ch;
            value+=chiter.next();
            while(chiter.hasData()){
                ch=chiter.peekSafe();
                if(ch=='\\'){
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
        }else if((ch=='+'||ch=='-') && nextch==ch){
            value+=ch;
            value+=ch;
            tokens.emplace_back(value);
            chiter.next();
        }else if(util::isIdent(ch)){
            value+=ch;
            while(chiter.hasData()){
                ch=chiter.peekSafe();
                if(util::isIdent(ch)){
                    value+=chiter.next();
                }else{
                    break;
                }
            }
            tokens.emplace_back(value);
        }else if(ch == L'\"'){
            value+=ch;
            while(chiter.hasData()){
                ch=chiter.next();
                if(ch=='\\'){
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
            if(util::isOp(ch)){
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
        }else if(ch==nextch && (ch == '<' || ch == '>') && chiter.peekSafe(1) == '='){
            chiter.next();
            chiter.next();

            value+=ch;
            value+=ch;
            value+='=';
            tokens.emplace_back(value);
        }else if((util::isMathOp(ch) || util::isBitOp(ch)) && nextch == '='){
            chiter.next();

            value+=ch;
            value+='=';
            tokens.emplace_back(value);
        }else if(ch==L'\n' || ch==L' '){
            // passing!
        }else{
            value+=ch;
            tokens.emplace_back(value);
        }
    }
}

std::wstring parserWrap::compile(){
    ctx.iter=iterator<std::wstring>(tokens);
    parserCore::program(ctx);
    std::wstring compiled= ctx.Asm->ss.str();
    ctx.stream.str(L"");
    ctx.stream.clear(std::stringstream::goodbit);
    return compiled;
}

std::wstring parserWrap::compile(std::wstring source){
    clear();
    set(source);
    tokenize();
    return compile();
}

void parserWrap::reset(){
    this->ctx.puts.clear();
    this->ctx.variables.clear();

    clear();
}
void parserWrap::clear(){
    this->string.clear();
    this->tokens.clear();

    delete this->ctx.Asm;
    this->ctx.Asm=new Assembly(this->ctx.stream);
    
    this->ctx.iter.clear();
    this->ctx.stream.str(L"");
    this->ctx.stream.clear(std::stringstream::goodbit);
}