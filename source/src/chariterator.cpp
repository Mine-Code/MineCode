#include <chariterator.h>
#include <iostream>

chariterator::chariterator(std::wstring src)
    :str(src), index(0)
{}

wchar_t chariterator::next(){
    if(str.length() <= index+1){
        std::cerr << "Out of size" << std::endl;;
        throw L"outofsize";
    }
    index++;
    return str[index];
}

wchar_t chariterator::prev(){
    if(index <= 0){
        std::cerr << "Out of size" << std::endl;;
        throw L"outofsize";
    }
    index--;
    return str[index];
}

wchar_t chariterator::peek(){
    return str[index];
}

bool chariterator::hasData(){
    if(str.length() <= index){
        return false;
    }
    return true;
}