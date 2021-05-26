#include <stmtProcessor.h>
#include <parserTypes.h>
#include <parserCore.h>
#include <util.h>
#include <syntaxError.h>
#include <eval.h>
#include <condeval.h>
#include <condutil.h>
#include <parserCore.h>

using namespace synErr;
using namespace parserTypes;

void stmtProcessor::For(parserCore *that, std::wstring target, std::wstring iter)
{
    std::wcout << "for iterator " << target << " in " << iter << std::endl;
    while (that->iter.hasData())
    {
        if (that->iter.peek() == L"}")
            break;
        that->stmt();
    }
}

void stmtProcessor::Forr(parserCore *that, int start, int end)
{
    that->Asm->startOfLoop(end - start, start);
    while (that->iter.hasData())
    {
        if (that->iter.peek() == L"}")
            break;
        that->stmt();
    }
    that->Asm->endOfLoop();
}

void stmtProcessor::While(parserCore *that, cond conditional)
{
    int id = that->Asm->whileBegin();
    while (that->iter.hasData())
    {
        if (that->iter.peek() == L"}")
            break;
        that->stmt();
    }
    std::wstring outerWhile = that->Asm->getEnd_While(id);
    condeval::Cond(that, conditional, L"", outerWhile);

    that->Asm->whileEnd(id);
}

void stmtProcessor::If(parserCore *that, struct cond conditional)
{
    condeval::Cond(that, conditional);

    while (that->iter.hasData())
    {
        if (that->iter.peek() == L"}")
            break;
        that->stmt();
    }
    that->Asm->endOfIf();
}

void stmtProcessor::Func(parserCore *that)
{
    that->stream << "# Inner Function\n";
    while (that->iter.hasData())
    {
        if (that->iter.peek() == L"}")
            break;
        that->stmt();
    }
    that->stream << "# Outer Function\n";
}

void stmtProcessor::Put(parserCore *)
{
}

void stmtProcessor::Assign(parserCore *that, value _target, std::wstring op, struct expr &val)
{
    if (_target.type == value::IDENT)
    {
        std::string target = util::wstr2str(_target.ident);
        // check: is avail variable of target
        if (that->variables.count(target) == 0)
        {
            // check: is [op==equal and not have element]
            if (op == L"=")
            {
                // make variable
                varType var;

                that->Asm->stack_offset += 4;
                var.offset = that->Asm->stack_size - that->Asm->stack_offset + 4;

                that->variables[target] = var;
            }
            else
            {
                processError(that, _target.ident + L" is not found", __FILE__, __func__, __LINE__);
            }
        }
    }
    else if (_target.type == value::PTR)
    {
        eval::Ptr_Addr(that, _target.pointer, 13);
    }
    else
    {
        processError(that, std::to_wstring(_target.type) + L" is not implemented...", __FILE__, __func__, __LINE__);
    }
    // Load value
    eval::Expr(that, val, 14);

    if (op == L"++")
    {
        that->Asm->add(1, 14, 14);
    }
    else if (op == L"--")
    {
        that->Asm->add(-1, 14, 14);
    }
    else if (op == L"=")
    {
    }
    else if ((util::isMathOp(op[0]) || util::isBitOpFull(op.substr(0, op.length() - 1))) && op[op.length() - 1] == L'=')
    {
        op = op.substr(0, op.length() - 1);
        if (op == L"+")
        {
            that->Asm->addReg(14);
        }
        else if (op == L"-")
        {
            that->Asm->subReg(14);
        }
        else if (op == L"/")
        {
            that->Asm->divReg(14);
        }
        else if (op == L"*")
        {
            that->Asm->mulReg(14);
        }
        else if (op == L"%")
        {
            that->Asm->modReg(14);
        }
        else if (op == L"<<")
        {
            that->Asm->shtlReg(14);
        }
        else if (op == L">>")
        {
            that->Asm->shtrReg(14);
        }
        else
        {
            processError(that, L"Unknown operator type: " + op, __FILE__, __func__, __LINE__);
        }
    }
    if (_target.type == value::IDENT)
    {
        std::string target = util::wstr2str(_target.ident);
        // check: is avail variable of target
        that->Asm->poke(that->variables[target].offset, 1, 14);
    }
    else if (_target.type == value::PTR)
    {
        that->Asm->poke(0, 13, 14);
    }
    else
    {
        processError(that, _target.type + L" is not implemented...", __FILE__, __func__, __LINE__);
    }
}

void stmtProcessor::executeFunction(parserCore *that, ExecFunc call)
{
    //load arguments
    int n = 3;
    for (auto arg : call.args)
    {
        eval::Expr(that, arg, n++);
    }
    //load address
    if (call.type == ExecFunc::ADDRESS)
    {
        // address based
        that->Asm->writeRegister(call.funcAddr, 15);
    }
    else if (call.type == ExecFunc::Name)
    {
        // name based
        if (that->variables.count(util::wstr2str(call.funcId)) == 1)
        {
            that->Asm->pop(that->variables[util::wstr2str(call.funcId)].offset, 15);
        }
        else if (that->functions.count(util::wstr2str(call.funcId)) == 1)
        {

            if (that->variables.count(util::wstr2str(call.funcId)) == 0)
            {
                processError(that, call.funcId + L" is not found", __FILE__, __func__, __LINE__);
            }
            that->Asm->writeRegister(that->functions[util::wstr2str(call.funcId)].addr, 15);
        }
        else
        {
            processError(that, L"Function not found: " + call.funcId, __FILE__, __func__, __LINE__);
        }
    }
    that->stream << "mtctr r15\n"
                    "bctrl\n";
}