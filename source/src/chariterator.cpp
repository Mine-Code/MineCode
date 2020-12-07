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