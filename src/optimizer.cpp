#include <optimizer.h>
#include <typedIterator.hxx>

Iasm* convert(std::wstring asm){
    
}

optimizer::optimizer(std::wstring src){
    std::wstring line;
    for(auto ch: src){
        line+=ch;
        if(ch=='\n'){
            codes.emplace_back(line);
            line=L"";
        }
    }
}

std::vector<std::wstring> optimizer::get(){
    iterator<std::wstring> iter;
    // convert to Iasm and subclass
}