#include <iostream>
#include <asm.h>

int main()
{
    std::wstringstream ss;
    Assembly assembly(ss);
    assembly.startOfFunction();
    assembly.callFunction(0x10000000);
    assembly.endOfFunction();
    std::wcout << ss.str() << std::endl;
}