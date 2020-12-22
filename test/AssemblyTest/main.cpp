#include <iostream>
#include <asm.h>

int main()
{
    std::wstringstream ss;
    Assembly assembly(ss);
    assembly.startOfFunction();
    assembly.peek(0x1EAF000C);
    assembly.mul(2);
    assembly.poke(0x10000000);
    assembly.endOfFunction();
    std::wcout << ss.str() << std::endl;
}