#include <iostream>
#include <asm.h>

int main()
{
    std::wstringstream ss;
    Assembly assembly(ss);
    assembly.startOfFunction();

    assembly.setString(L"test");

    assembly.endOfFunction();
    std::wcout << ss.str() << std::endl;
}