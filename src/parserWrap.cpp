#include <parserWrap.h>
#include <iostream>
#include <typedIterator.hxx>
#include <util.h>
#include <parserCore.h>
#include <parserContext.h>

void parserWrap::debug(){
    
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
            while(1){
                ch=std::tolower(chiter.peek());
                if(!chiter.hasData()){
                    error_program(chiter);
                }else if(util::isHex(ch)){
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
            while(1){
                ch=chiter.peek();
                if(!chiter.hasData()){
                    error_program(chiter);
                }else if(util::inRange<char>('0',ch,'8')){
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
            while(1){
                ch=chiter.peek();
                if(!chiter.hasData()){
                    error_program(chiter);
                }
                else if(util::isDec(ch)){
                    value+=chiter.next();
                }else{
                    break;
                }
            }
            tokens.emplace_back(value);
        }else if(ch==L'=' && nextch==L'='){
            chiter.next(); //skip one equal
            tokens.emplace_back(L"==");
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
        }else if((ch=='+'||ch=='-') && nextch==ch){
            value+=ch;
            value+=ch;
            tokens.emplace_back(value);
            chiter.next();
        }else if(iswalpha(ch)){
            value+=ch;
            while(1){
                ch=chiter.peek();
                if(!chiter.hasData()){
                    error_program(chiter);
                }else if(iswalpha(ch)){
                    value+=chiter.next();
                }else{
                    break;
                }
            }
            tokens.emplace_back(value);
        }else if(ch == L'\"'){
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
        }else if(ch==L'\n' || ch==L' '){
            // passing!
        }else{
            value+=ch;
            tokens.emplace_back(value);
        }
    }
}

void parserWrap::parse(){
    parserCtx::parserContext ctx;
    ctx.iter=iterator<std::wstring>(tokens);
    
    parserCore::program(ctx);
    // process 'st.str()'
    std::wcout<<ctx.Asm->ss.str()<<std::endl;
}