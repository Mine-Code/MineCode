#include <iostream>
#include <asm.h>

int main()
{
    std::wstringstream ss;
    Assembly assembly(ss);
    assembly.startOfFunction();

    //[0x10000000] = 2**10
    assembly.writeRegister(1, 13);
    assembly.startOfLoop(10);
    assembly.mul(2);
    assembly.endOfLoop();
    assembly.writeRegister(0x10000000, 12);
    assembly.poke();

    //[0x10000004] = 3**10
    assembly.writeRegister(1, 13);
    assembly.startOfLoop(10);
    assembly.mul(3);
    assembly.endOfLoop();
    assembly.writeRegister(0x10000004, 12);
    assembly.poke();

    assembly.endOfFunction();
    std::wcout << ss.str() << std::endl;
}