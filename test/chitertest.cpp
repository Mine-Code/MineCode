#include <chariterator.h>
#include <iostream>

using namespace std::literals::string_literals;

int main()
{
    chariterator chariter(L"abcdef"s);
    while(chariter.hasData()){
        std::wcout<<chariter.next()<<std::endl;
    }
}
