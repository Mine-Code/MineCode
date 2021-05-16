#include <eval.h>

#include <parserTypes.h>
#include <syntaxError.h>
#include <stmtProcessor.h>
#include <util.h>
#include <parserCore.h>

using namespace parserTypes;

void Ptr_AddrBase(parserCore *that, ptr obj, int dest)
{
    eval::Expr(that, obj.getBase(), dest);
}

term &optimize(term &val)
{
    int immutable_mul = 0;
    int immutable_div = 0;
    term newTerm;
    // process of mul
    for (auto part : val.parts)
    {
        if (part.value.isSingle() && part.value.parts[0].type == power::IMM)
        {
            auto imm = part.value.parts[0].imm;
            if (part.type == expo_wrap::MUL)
                immutable_mul *= imm;
            else if (part.type == expo_wrap::DIV)
                immutable_div *= imm;
            else if (part.type == expo_wrap::MOD)
                newTerm.parts.emplace_back(part);
        }
        else
        {
            newTerm.parts.emplace_back(part);
        }
    }

    val.parts = newTerm.parts;

    // add mul/div element
    { // add mul
        power powerElem;
        powerElem.type = power::IMM;
        powerElem.imm = immutable_mul;

        expo expoElem;
        expoElem.parts.emplace_back(powerElem);

        expo_wrap wrapElem;
        wrapElem.type = expo_wrap::MUL;
        wrapElem.value = expoElem;

        val.parts.emplace_back(wrapElem);
    }
    { // add div
        power powerElem;
        powerElem.type = power::IMM;
        powerElem.imm = immutable_div;

        expo expoElem;
        expoElem.parts.emplace_back(powerElem);

        expo_wrap wrapElem;
        wrapElem.type = expo_wrap::DIV;
        wrapElem.value = expoElem;

        val.parts.emplace_back(wrapElem);
    }

    return val;
}

expr &optimize(expr &val)
{
    int immutable = 0;
    expr newExpr;
    for (auto _part : val.parts)
    {
        auto part = optimize(_part);
        if (part.isSingle() && part.parts[0].value.isSingle() && part.parts[0].value.parts[0].type == power::IMM)
        {
            immutable += part.parts[0].value.parts[0].imm;
        }
        else
        {
            newExpr.parts.emplace_back(part);
        }
    }
    if (immutable != 0)
    {
        power value;
        value.type = power::IMM;
        value.imm = immutable;

        expo exponent;
        exponent.parts.emplace_back(value);

        expo_wrap wrap;
        wrap.type = expo_wrap::MUL;
        wrap.value = exponent;

        term Term;
        Term.parts.emplace_back(wrap);

        val.parts.emplace_back(Term);
    }

    val.parts = newExpr.parts; // copy new expr (stack safe)
    return val;
}

void eval::Expr(parserCore *that, expr obj, int dest)
{
    int offs = that->Asm->stack_offset;
    std::vector<int> stackOffsets;

    if (obj.isSingle())
    {
        Term(that, obj.parts[0], dest);
    }
    else
    {
        // write all
        for (auto elem : obj.parts)
        {
            Term(that, elem, dest);
            stackOffsets.emplace_back(that->Asm->push(dest));
        }

        that->Asm->writeRegister(0, dest);
        for (auto i : stackOffsets)
        {
            that->Asm->pop(i, 14);
            that->stream << "add r" << dest << ", r" << dest << ", r14" << std::endl;
        }
    }

    that->Asm->stack_offset = offs;
}
void eval::Expo(parserCore *that, expo obj, int dest)
{
    int offs = that->Asm->stack_offset;
    std::vector<int> stackOffsets;

    if (obj.isSingle())
    {
        Power(that, obj.parts[0], dest);
    }
    else
    {
        // write all
        for (auto elem : obj.parts)
        {
            Power(that, elem, dest);
            stackOffsets.emplace_back(that->Asm->push(dest));
        }
        // TODO: power all stackOffsets
    }
    that->Asm->stack_offset = offs;
}
void eval::Term(parserCore *that, term obj, int dest)
{
    int offs = that->Asm->stack_offset;
    struct offset
    {
        enum Type
        {
            MUL,
            DIV,
            MOD
        };

        Type type;
        int offset;
    };
    std::vector<struct offset> stackOffsets;

    if (obj.isSingle())
    {
        Expo(that, obj.parts[0].value, dest);
    }
    else
    {
        // write all
        for (auto elem : obj.parts)
        {
            Expo(that, elem.value, dest);

            int offset = that->Asm->push(dest);
            offset::Type type;
            switch (elem.type)
            {
            case expo_wrap::MUL:
                type = offset::Type::MUL;
                break;
            case expo_wrap::DIV:
                type = offset::Type::DIV;
                break;
            case expo_wrap::MOD:
                type = offset::Type::MOD;
                break;
            default:
                synErr::processError(that, L"Unknown expr_wrap type ", __FILE__, __func__, __LINE__);
                std::terminate(); // dead code
            }

            struct offset element;
            element.type = type;
            element.offset = offset;

            stackOffsets.emplace_back(element);
        }

        that->Asm->writeRegister(1, dest); // init dest
        for (auto a : stackOffsets)
        {
            that->Asm->pop(a.offset, 14);
            switch (a.type)
            {
            case offset::MUL:
                that->stream << "mullw r" << dest << ", r" << dest << ", r14" << std::endl;
                break;
            case offset::DIV:
                that->stream << "divw r" << dest << ", r" << dest << ", r14" << std::endl;
                break;
            case offset::MOD:
                that->stream << "#mod r" << dest << ", r" << dest << ", r14" << std::endl;
                break;
            }
        }
    }

    that->Asm->stack_offset = offs;
}
void eval::Power(parserCore *that, power obj, int dest)
{
    int offs = that->Asm->stack_offset;
    switch (obj.type)
    {
    case power::EXPR:
        Expr(that, obj.expr, dest);
        break;
    case power::FLT:
        synErr::processError(that, L"Float isn't supported...", __FILE__, __func__, __LINE__);
        break;
    case power::FUNCCALL:
        stmtProcessor::executeFunction(that, *obj.func);
        that->Asm->moveResister(3, dest);
        break;
    case power::IMM:
        that->Asm->writeRegister(obj.imm, dest);
        break;
    case power::PTR:
        Ptr(that, obj.Pointer, dest);
        break;
    case power::VAR:
        Var(that, obj.var, dest);
        break;
    default:
        synErr::processError(that,
                             L"unknown type error [" + std::to_wstring(obj.type) + L"]", __FILE__, __func__, __LINE__);
        break;
    }
    that->Asm->stack_offset = offs;
}
void eval::Ptr(parserCore *that, ptr obj, int dest)
{
    int offs = that->Asm->stack_offset;
    Ptr_AddrBase(that, obj, dest);
    that->Asm->peek(0, dest, dest);
    that->Asm->stack_offset = offs;
}
void eval::Ptr_Addr(parserCore *that, ptr obj, int dest)
{
    Ptr_AddrBase(that, obj, dest);
}
void eval::Var(parserCore *that, std::wstring obj, int dest)
{
    if (obj[0] == '\"')
    {
        that->Asm->setString(obj, dest);
    }
    else if (obj == L"false")
    {
        that->Asm->writeRegister(0, dest);
    }
    else if (obj == L"true")
    {
        that->Asm->writeRegister(1, dest);
    }
    else
    {
        if (that->variables.count(util::wstr2str(obj)) == 1)
        {
            that->Asm->pop(that->variables[util::wstr2str(obj)].offset, dest);
        }
        else
        {
            synErr::processError(that, L"variable not found: " + obj, __FILE__, __func__, __LINE__);
        }
    }
}