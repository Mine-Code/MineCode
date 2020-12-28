#include <iostream>
#include <asm.h>

int main()
{
    std::wstringstream ss;
    Assembly assembly(ss);
    assembly.startOfFunction();

    assembly.writeRegister(5, 13);
    int offset_1 = assembly.push();
    assembly.writeRegister(5, 13);
    int offset_2 = assembly.push();
    assembly.pop(offset_1, 3);
    assembly.pop(offset_2, 4);
    ss << "add r3, r3, r4\n";
    assembly.writeRegister(0x10000000, 12);
    assembly.poke();

    assembly.endOfFunction();
    std::wcout << ss.str() << std::endl;
}