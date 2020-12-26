#include <sstream>
#include <cassert>
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

void Assembly::writeRegister(int value, int dest)
{
    assert((unsigned int)value < 0xFFFFFFFF);

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

void Assembly::push(int offset, int src)
{
    ss <<
        "stw r" << src << ", " << offset << "(r1)";
}

void Assembly::pop(int offset, int dest)
{
    ss <<
        "stw r" << dest << ", " << offset << "(r1)";
}

void Assembly::callFunction(int address)
{
    writeRegister(address, 12);
    ss <<
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
    writeRegister(address, 12);
    ss <<
        "lwz r" << dest << ", " << offset << "(r12)\n";
}

void Assembly::poke(int offset, int dest, int src)
{
    ss <<
        "stw r" << src << ", " << offset << "(r" << dest << ")\n";
}

void Assembly::poke_i(int address, int src, int offset)
{
    writeRegister(address, 12);
    ss <<
        "stw r" << src << ", " << offset << "(r12)\n";
}

void Assembly::add(int value, int src, int dest)
{
    writeRegister(value, 3);
    ss <<
        "add r" << dest << ", r3" << ", r" << src << "\n"; //r(dest) = r3 + r(src)
}
void Assembly::mul(int value, int src, int dest)
{
    writeRegister(value, 3);
    ss <<
        "mullw r" << dest << ", r3" << ", r" << src << "\n"; //r(dest) = r3 + r(src)
}

void Assembly::div(int value, int src, int dest)
{
    writeRegister(value, 3);
    ss <<
        "divw r" << dest << ", r3" << ", r" << src << "\n"; //r(dest) = r3 + r(src)
}

void Assembly::OR(int src_1, int dest, int src_2)
{
    ss <<
        "or r" << dest << ", r" << src_1 << ", r" << src_2 << "\n";
}

void Assembly::XOR(int src_1, int dest, int src_2)
{
    ss <<
        "xor r" << dest << ", r" << src_1 << ", r" << src_2 << "\n";
}

void Assembly::AND(int src_1, int dest, int src_2)
{
    ss <<
        "and r" << dest << ", r" << src_1 << ", r" << src_2 << "\n";
}

void Assembly::rightShift(int bit, int src, int dest)
{
    writeRegister(bit, 3);
    ss <<
        "srw r" << dest << ", r" << src << ", r3\n";
}

void Assembly::leftShift(int bit, int src, int dest)
{
    writeRegister(bit, 3);
    ss <<
        "slw r" << dest << ", r" << src << ", r3\n";
}

void Assembly::startOfLoop(int count)
{
    loop_count = count;
    loop_flag = 0;

    ss <<
        "stw r" << make_loop_ctr + 14 << ", " << stack_size - 4 * (make_loop_ctr + 2) << "(r1)\n"
        "li r" << make_loop_ctr + 14 << ", 0\n"
        "loop_" << make_loop_ctr << ":\n";
    make_loop_ctr++;
}

void Assembly::endOfLoop()
{
    int flag_register = make_loop_ctr + 13;
    ss <<
        "addi r" << flag_register << ", r" << flag_register << ", 1\n"
        "cmplwi r" << flag_register << ", " << loop_count << "\n"
        "blt loop_" << make_loop_ctr - 1 << "\n"
        "lwz r" << flag_register << ", " << stack_size - 4 * (make_loop_ctr + 1) << "(r1)\n";
    make_loop_ctr--;
}

void Assembly::startOfIf(int src_1, std::string mode, int src_2)
{
    ss <<
        "cmplw r" << src_1 << ", r" << src_2 << "\n";
    if (mode == "==") {
        ss << "beq if_" << make_if_ctr << "\n";
    } else if (mode == "!=") {
        ss << "beq if_" << make_if_ctr << "\n";
    } else if (mode == "<") {
        ss << "blt if_" << make_if_ctr << "\n";
    } else if (mode == "<=") {
        ss << "ble if_" << make_if_ctr << "\n";
    } else if (mode == ">") {
        ss << "bgt if_" << make_if_ctr << "\n";
    } else if (mode == ">=") {
        ss << "bge if_" << make_if_ctr << "\n";
    }
    ss <<
        "b " << "endif_" << make_if_ctr << ":\n"
        "if_" << make_if_ctr << ":\n";
    make_if_ctr++;
}

void Assembly::endOfIf()
{
    ss <<
        "endif_" << make_if_ctr - 1 << ":\n";
    make_if_ctr--;
}
