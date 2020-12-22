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
        "lmw r13, " << "-" << stack_size << "(r1) \n"
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

void Assembly::add(int value, int src, int dest)
{
    int high = ((uint16_t*)&value)[1];
    int low = ((uint16_t*)&value)[0];

    if (high != 0) {
        ss <<
            "lis r3, " << high << "\n"
            "ori r3, r3, " << low << "\n" //r3 = value
            "add r" << dest << ", r3" << ", r" << src << "\n"; //r(dest) = r3 + r(src)
    } else if (low != 0) {
        ss <<
            "li r3, " << low << "\n"
            "add r" << dest << ", r3" << ", r" << src << "\n";
    }
}
void Assembly::mul(int value, int src, int dest)
{
    int high = ((uint16_t*)&value)[1];
    int low = ((uint16_t*)&value)[0];

    if (high != 0) {
        ss <<
            "lis r3, " << high << "\n"
            "ori r3, r3, " << low << "\n" //r3 = value
            "mullw r" << dest << ", r3" << ", r" << src << "\n"; //r(dest) = r3 * r(src)
    } else if (low != 0) {
        ss <<
            "li r3, " << low << "\n"
            "mullw r" << dest << ", r3" << ", r" << src << "\n";
    }
}

