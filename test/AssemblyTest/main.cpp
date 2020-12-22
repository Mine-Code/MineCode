#include <iostream>
#include <asm.h>

int main()
{
    std::wstringstream ss;
    Assembly assembly(ss);
    assembly.startOfFunction();
    ss << "li r13, 1";
    assembly.add(1);
    assembly.endOfFunction();
    std::wcout << ss.str() << std::endl;
}