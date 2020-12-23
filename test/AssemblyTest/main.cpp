#include <iostream>
#include <asm.h>

int main()
{
    //FreeFly [[0x109CD8E4] + 0x34] + 0x70C = 0x01000100
    std::wstringstream ss;
    Assembly assembly(ss);
    assembly.startOfFunction();
    assembly.peek(0x109CD8E4);
    assembly.peek(0, true, 0x34, 13, 12);
    assembly.writeResister(0x01000101, 13);
    assembly.poke(0, false, 0x70C);
    assembly.endOfFunction();
    std::wcout << ss.str() << std::endl;
}