#include <sstream>
#include <asm.h>


Assembly::Assembly(std::wstringstream& _ss)
    : ss(_ss)
{
}

void Assembly::startOfFunction(std::wstringstream& ss)
{
    ss <<
        "stmw r13, " << "-" << stack_size << "(r1)\n"
        "subi r1, r1, " << stack_size << "\n"
        "mflr r0 \n"
        "stw r0, " << stack_size + 4 << "(r1) \n";
}

void Assembly::endOfFunction(std::wstringstream& ss)
{
    ss <<
        "addi r1, r1, " << stack_size << "\n"
        "lmw r13, " << " - " << stack_size << "(r1) \n"
        "lwz r0, " << stack_size + 4 << "(r1) \n"
        "mtlr r0 \n"
        "blr \n";
}

void Assembly::callFunction(std::wstringstream& ss, int address)
{
    ss <<
        "lis r12, " << ((uint16_t*)&address)[0] << "\n"
        "ori r12, r12, " << ((uint16_t*)&address)[1] << "\n"
        "mtctr r12 \n"
        "bctrl \n";
}