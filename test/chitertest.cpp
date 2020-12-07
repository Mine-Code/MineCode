#include <chariterator.h>
#include <iostream>

using namespace std::literals::string_literals;

int main()
{
    chariterator chariter(L"abcdef"s);
    std::wcout
        << chariter.next()
        << chariter.next()
        << chariter.next()
        << chariter.next()
        << chariter.next()
        << std::endl;
    //while(chariter.hasData()){
    //    std::wcout<<chariter.next()<<std::endl;
    //}
}
