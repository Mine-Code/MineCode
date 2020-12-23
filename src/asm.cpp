#include <sstream>
#include <asm.h>


Assembly::Assembly(std::wstringstream& _ss)
    : ss(_ss)
{
}

void Assembly::startOfFunction()
{
    ss <<
        "stwu r1, -" << stack_size << "(r1)\n"
        "mflr r0\n"
        "stw r0, " << stack_size + 4 << "(r1)\n"
        "stw r13, " << stack_size - 4 << "(r1)\n";
}

void Assembly::endOfFunction()
{
    ss <<
        "lwz r0, " << stack_size + 4 << "(r1)\n"
        "lwz r13, " << stack_size - 4 << "(r1)\n"
        "mtlr r0\n"
        "addi r1, r1, " << stack_size << "\n"
        "blr";
}

void Assembly::writeResister(int value, int dest)
{
    int high = ((uint16_t*)&value)[1];
    int low = ((uint16_t*)&value)[0];

    if (high != 0) {
        ss <<
            "lis r" << dest << ", " << ((uint16_t*)&value)[1] << "\n"
            "ori r" << dest << ", r" << dest << ", " << ((uint16_t*)&value)[0] << "\n";
    } else {
        ss <<
            "li r" << dest << ", " << ((uint16_t*)&value)[0] << "\n";
    }
}

void Assembly::moveResister(int src, int dest)
{
    ss <<
        "mr r" << dest << ", r" << src << "\n";
}

void Assembly::callFunction(int address)
{
    ss <<
        "lis r12, " << ((uint16_t*)&address)[1] << "\n"
        "ori r12, r12, " << ((uint16_t*)&address)[0] << "\n"
        "mtctr r12 \n"
        "bctrl \n";
}

void Assembly::peek(int offset, int dest, int src)
{
    ss <<
        "mr r12, r" << src << "\n"
        "lwz r" << dest << ", " << offset << "(r12)\n";
}

void Assembly::peek_i(int address, int offset, int dest)
{
    ss <<
        "lis r12, " << ((uint16_t*)&address)[1] << "\n"
        "ori r12, r12, " << ((uint16_t*)&address)[0] << "\n"
        "lwz r" << dest << ", " << offset << "(r12)\n";
}

void Assembly::poke(int offset, int dest, int src)
{
    ss <<
        "stw r" << src << ", " << offset << "(r" << dest << ")\n";
}

void Assembly::poke_i(int address, int src, int offset)
{
    int high = ((uint16_t*)&address)[1];
    int low = ((uint16_t*)&address)[0];

    if (low != 0) {
        ss <<
            "lis r12, " << high << "\n"
            "ori r12, r12, " << low << "\n"
            "stw r" << src << ", " << offset << "(r12)\n";
    } else {
        ss <<
            "lis r12, " << high << "\n"
            "stw r" << src << ", " << offset << "(r12)\n";
    }
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

void Assembly::div(int value, int src, int dest)
{
    int high = ((uint16_t*)&value)[1];
    int low = ((uint16_t*)&value)[0];

    if (high != 0) {
        ss <<
            "lis r3, " << high << "\n"
            "ori r3, r3, " << low << "\n" //r3 = value
            "divw r" << dest << ", r3" << ", r" << src << "\n"; //r(dest) = r3 * r(src)
    } else if (low != 0) {
        ss <<
            "li r3, " << low << "\n"
            "divw r" << dest << ", r3" << ", r" << src << "\n";
    }
}

void Assembly::startOfLoop(int count)
{
    loop_count = count;
    loop_flag = 0;

    ss <<
        "stw r14, " << stack_size - 8 << "(r1)\n"
        "li r14, 0\n"
        "loop_" << make_loop_ctr << ":\n";
}

void Assembly::endOfLoop()
{
    ss <<
        "addi r14, r14, 1\n"
        "cmplwi r14, " << loop_count << "\n"
        "blt loop_" << make_loop_ctr << "\n"
        "lwz r14, " << stack_size - 8 << "(r1)\n";

    make_loop_ctr++;
}
