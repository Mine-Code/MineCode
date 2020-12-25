#include <iostream>
#include <asm.h>

int main()
{
    std::wstringstream ss;
    Assembly assembly(ss);
    assembly.startOfFunction();

    assembly.writeRegister(13, 1);
    assembly.startOfLoop(5);
    assembly.startOfLoop(5);
    assembly.mul(2);
    assembly.endOfLoop();
    assembly.endOfLoop();
    assembly.writeRegister(0x10000000, 12);
    assembly.poke();

    assembly.endOfFunction();
    std::wcout << ss.str() << std::endl;
}