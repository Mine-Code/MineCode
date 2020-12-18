#include <asm.h>

void assembly::Assembly::startOfFunction(std::wstringstream& st)
{
    st <<
        "stmw r13, " << "-" << stack_size << "(r1)\n"
        "subi r1, r1, " << stack_size << "\n"
        "mflr r0 \n"
        "stw r0, " << stack_size + 4 << "(r1) \n";
}

void assembly::Assembly::endOfFunction(std::wstringstream& st)
{
    st <<
        "addi r1, r1, " << stack_size << "\n"
        "lmw r13, " << " - " << stack_size << "(r1) \n"
        "lwz r0, " << stack_size + 4 << "(r1) \n"
        "mtlr r0 \n"
        "blr \n";
}

void assembly::Assembly::callFunction(std::wstringstream& st, int address)
{
    st <<
        "lis r12, " << ((uint16_t*)&address)[0] << "\n"
        "ori r12, r12, " << ((uint16_t*)&address)[1] << "\n"
        "mtctr r12"
        "bctrl";
}
