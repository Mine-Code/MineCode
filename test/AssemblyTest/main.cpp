#include <iostream>
#include <asm.h>

int main()
{
    std::wstringstream ss;
    Assembly assembly(ss);
    assembly.startOfFunction();
    ss << "li r13, 2\n";
    assembly.mul(2);
    assembly.poke(0x10000000);
    assembly.endOfFunction();
    std::wcout << ss.str() << std::endl;
}