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
        << std::endl
        << chariter.prev()
        << chariter.prev()
        << chariter.prev()
        << std::endl
        << chariter.peek()
        << chariter.peek()
        << chariter.peek()
        << std::endl;
    //while(chariter.hasData()){
    //    std::wcout<<chariter.next()<<std::endl;
    //}
}
