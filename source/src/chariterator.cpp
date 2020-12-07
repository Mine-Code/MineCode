#include <chariterator.h>
#include <iostream>

chariterator::chariterator(std::wstring src)
    :str(src), index(0)
{}

wchar_t chariterator::next(){
    if(str.length() <= index){
        std::cerr << "Out of size" << std::endl;;
        throw L"outofsize";
    }
    return str[index++];
}

wchar_t chariterator::prev(){
    if(index <= 0){
        std::cerr << "Out of size" << std::endl;;
        throw L"outofsize";
    }
    return str[index--];
}

wchar_t chariterator::peek(){
    return str[index];
}

wchar_t chariterator::peek(size_t offset){
    if(index <= 0 || str.length() <= index){
        std::cerr << "Out of size" << std::endl;;
        throw L"outofsize";
    }
    return str[index+offset];
}

bool chariterator::hasData(){
    if(str.length() <= index){
        return false;
    }
    return true;
}