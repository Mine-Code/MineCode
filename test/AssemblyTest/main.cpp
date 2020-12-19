#include <iostream>
#include <asm.h>

int main()
{
    std::wstringstream ss;
    Assembly assembly(ss);
    assembly.startOfFunction(ss);
    assembly.callFunction(ss, 0x2420120);
    assembly.endOfFunction(ss);
    std::wcout << ss.str() << std::endl;
}