#include <iostream>
#include <asm.h>

int main()
{
    std::wstringstream ss;
    Assembly assembly(ss);
    assembly.startOfFunction();

    assembly.peek_i(0x109CD8E4);
    assembly.peek(0x34);
    int offset = assembly.push();
    assembly.startOfLoop(12345);
    assembly.pop(offset, 3);
    assembly.writeRegister(1, 4);
    assembly.callFunction(0x02725330);
    assembly.endOfLoop();

    assembly.endOfFunction();
    std::wcout << ss.str() << std::endl;
}