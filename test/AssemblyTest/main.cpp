#include <iostream>
#include <asm.h>

int main()
{
    std::wstringstream ss;
    Assembly assembly;
    assembly.startOfFunction(ss);
    assembly.endOfFunction(ss);
    std::wcout << ss.str() << std::endl;
}