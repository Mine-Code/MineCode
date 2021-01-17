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
    assert((unsigned int)value <= 0xFFFFFFFF);

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

int Assembly::push(int src)
{
    ss <<
        "stw r" << src << ", " << stack_size - stack_offset << "(r1)\n";
    stack_offset += 4;
    return stack_size - stack_offset + 4;
}



void Assembly::pop(int offset, int dest)
{
    ss <<
        "lwz r" << dest << ", " << offset << "(r1)\n";
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

void Assembly::startOfLoop(int count, int init)
{
    loop_count = count;
    loop_flag = 0;

    ss <<
        "li r3, " << init << "\n";
    int offset = push(3);
    ss <<
        "loop_" << make_loop_ctr << ":\n";
    loop_offset.emplace_back(offset);
    make_loop_ctr++;
}

void Assembly::endOfLoop()
{
    int offset = loop_offset[make_loop_ctr - 1];
    pop(offset, 3);
    ss <<
        "addi r3, r3, 1\n"
        "cmplwi r3, " << loop_count << "\n"
        "stw r3, " << offset << "(r1)\n"
        "blt loop_" << make_loop_ctr - 1 << "\n";
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

void Assembly::startOfIf(condType mode, int condReg){
    switch(mode){
        case EQU: // ==
            ss << "beq if_" << make_if_ctr << ", cr"<<condReg << "\n";
            break;
        case NEQ: // !=
            ss << "beq if_" << make_if_ctr << ", cr"<<condReg << "\n";
            break;
        case LT:  // < 
            ss << "blt if_" << make_if_ctr << ", cr"<<condReg << "\n";
            break;
        case GT:  // > 
            ss << "bgt if_" << make_if_ctr << ", cr"<<condReg << "\n";
            break;
        case GE:  // <=
            ss << "bge if_" << make_if_ctr << ", cr"<<condReg << "\n";
            break;
        case LE:  // >=
            ss << "ble if_" << make_if_ctr << ", cr"<<condReg << "\n";
            break;
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

void Assembly::compare(int reg1, int reg2){
    ss<<"cmplw r" << reg1 << ", r" << reg2 << "\n";
}
void Assembly::compareImm(int reg1, int imm){
    ss<<"cmplwi r" << reg1 << ", " << imm << "\n";
}
void Assembly::condJump(condType mode, int condReg,uint32_t addr){
    std::wstring mnemonic;
    switch(mode){
        case EQU: mnemonic=L"beqctrl"; break;
        case NEQ: mnemonic=L"beqctrl"; break;
        case LT:  mnemonic=L"bltctrl"; break;
        case GT:  mnemonic=L"bgtctrl"; break;
        case GE:  mnemonic=L"bgectrl"; break;
        case LE:  mnemonic=L"blectrl"; break;
    }
    writeRegister(addr,12);
    ss<< "mtctr r12\n"
      << mnemonic<< "\n";
}
void Assembly::condJump(condType mode, int condReg,std::wstring label){
    std::wstring mnemonic;
    switch(mode){
        case EQU: mnemonic=L"beq"; break;
        case NEQ: mnemonic=L"beq"; break;
        case LT:  mnemonic=L"blt"; break;
        case GT:  mnemonic=L"bgt"; break;
        case GE:  mnemonic=L"bge"; break;
        case LE:  mnemonic=L"ble"; break;
    }
    ss<< mnemonic<< " " <<label <<"\n";
}
std::wstring Assembly::makeLabel(std::wstring name){
    ss<<name<<":";
    return name;
}
std::wstring Assembly::makeLabel(){
    static int no=0;
    ss<<"L"<<no<<":\n";
    return L"L"+no;
}