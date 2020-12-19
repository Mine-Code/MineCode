#include <sstream>
#include <asm.h>


Assembly::Assembly(std::wstringstream& _ss)
    : ss(_ss)
{
}

void Assembly::startOfFunction()
{
    ss <<
        "stmw r13, " << "-" << stack_size << "(r1)\n"
        "subi r1, r1, " << stack_size << "\n"
        "mflr r0 \n"
        "stw r0, " << stack_size + 4 << "(r1) \n";
}

void Assembly::endOfFunction()
{
    ss <<
        "addi r1, r1, " << stack_size << "\n"
        "lmw r13, " << " - " << stack_size << "(r1) \n"
        "lwz r0, " << stack_size + 4 << "(r1) \n"
        "mtlr r0 \n"
        "blr \n";
}

void Assembly::callFunction(int address)
{
    ss <<
        "lis r12, " << ((uint16_t*)&address)[1] << "\n"
        "ori r12, r12, " << ((uint16_t*)&address)[0] << "\n"
        "mtctr r12 \n"
        "bctrl \n";
}