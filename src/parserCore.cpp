#include <parserCore.h>

#include <numeric>

#include <parserTypes.h>
#include <stmtProcessor.h>
#include <util.h>
#include <syntaxError.h>
#include <myassert.h>
#include <mcl.h>
#include <parserWrap.h>

using namespace synErr;
using namespace parserTypes;
using namespace util;

std::wstring convPut(std::wstring src, std::wstring argument);

void parserCore::program()
{
    if (iter.peekSafe() == L"#")
    {
        stream << "# do once" << std::endl;
        assert(iter.next() == L"#", L"mismatch preprecessor operation");
        assert(iter.next() == L"do", L"mismatch preprecessor operation");
        assert(iter.next() == L"once", L"mismatch preprecessor operation");
        // TODO: implement do once
    }

    while (iter.hasData())
    {
        stmt();
    }
}
void parserCore::stmt()
{
    // stmt Switcher
    std::wstring text = iter.peek();
    if (text == L"for")
    {
        For();
        return;
    }
    if (text == L"while")
    {
        While();
        return;
    }
    if (text == L"if")
    {
        If();
        return;
    }
    if (text == L"mcl")
    {
        mcl();
        return;
    }
    if (text == L"return")
    {
        iter.next();
        Asm->Jump(L"__ret");
        return;
    }
    if (text == L"func")
    {
        if (iter.peekSafe(1) == L"[")
        {
            stmtProcessor::executeFunction(this, funcCall());
        }
        else
        {
            func();
        }
        return;
    }

    // skip one 'value' and read one
    auto backup = iter.index;
    value();
    text = iter.peek();
    iter.index = backup;
    // done skip and read
    if (text == L"<<")
    {
        put();
    }
    else if (text == L"(")
    {
        stmtProcessor::executeFunction(this, funcCall());
    }
    else if (isAssignOp(text))
    {
        assign();
    }
}
void parserCore::func()
{
    assertChar("func");
    std::wstring functionName = iter.next();
    stream << "# "
           << "funcName:" << functionName << std::endl;
    assertChar("(");

    // read arguments
    std::vector<Arg> args;
    // args=(type,name)

    if (iter.peek() != L")")
    {
        args.emplace_back(arg());
    }
    while (iter.peek() != L")")
    {
        assertChar(",");
        args.emplace_back(arg());
    }
    // end: read arguments
    assertChar(")");
    assertChar("{");
    stmtProcessor::Func(this);
    assertChar("}");
}
void parserCore::For()
{
    assertChar("for");
    std::wstring varname = iter.next();
    assertChar("in");
    stream << "# for ";
    if (iter.peek(1) == L"...")
    {
        stream << " range" << std::endl;
        Range target = range();
        assertChar("{");
        stmtProcessor::Forr(this, target.first, target.second);
    }
    else
    {
        stream << " iter " << std::endl;
        std::wstring target = ident();
        assertChar("{");

        stmtProcessor::For(this, varname, target);
    }
    assertChar("}");
}
void parserCore::put()
{
    std::string target = util::wstr2str(ident());
    assertChar("<<");
    // get end
    auto start = iter.index;
    struct expr val = expr();
    auto end = iter.index;
    iter.index = start;
    // get end
    std::wstring expression;
    iter.index = 0; // enable absolute get
    for (size_t i = start; i < end; i++)
    {
        expression += iter.peekSafe(i);
    }
    iter.index = end;
    assert(puts.count(target) == 1, L"Puts Not found");
    std::wstring source = util::str2wstr(puts[target]);
    source = convPut(source, expression);
    stream << compiler->compile(source);
}
parserCore::Arg parserCore::arg()
{
    return std::make_pair(
        iter.next(), // type
        iter.next()  // name
    );
}
struct value parserCore::value()
{
    struct value ret;
    wchar_t ch = iter.peek()[0];
    if (iter.peek() == L"[")
    {
        ret.type = value::PTR;
        ret.pointer = ptr();
    }
    else if (isalpha(ch))
    {
        ret.type = value::IDENT;
        ret.ident = ident();
    }
    else if (ch == L'"')
    {
        ret.type = value::STR;
        ret.str = iter.next();
    }
    else if (isdigit(ch))
    {
        ret.type = value::IMM;
        ret.imm = util::toInt(iter.next());
    }
    else
    {
        syntaxError(this, L"is not value type ", __FILE__, __func__, __LINE__);
        throw ""; // do not call this
    }
    return ret;
}
struct ptr parserCore::ptr()
{
    assertChar("[");
    struct expr *value = new struct expr;
    *value = expr();
    assertChar("]");
    return parserTypes::ptr(value);
}
struct value parserCore::editable()
{
    struct value ret;
    if (iter.peek() == L"[")
    {
        ret.type = value::PTR;
        ret.pointer = ptr();
    }
    else
    {
        ret.type = value::IDENT;
        ret.ident = ident();
    }
    return ret;
}
std::wstring parserCore::ident()
{
    std::wstring text = iter.next();
    // check word?
    if (!isalpha(text[0]))
    {
        processError(this, L"isn't ident", __FILE__, __func__, __LINE__);
    }
    // read under the `.`
    while (iter.peek() == L"." || iter.peek() == L"::")
    {
        std::wstring joint = iter.next();
        text += joint + iter.next();
    }
    return text;
}
struct value parserCore::constant()
{
    struct value ret;
    std::wstring text = iter.next();
    // check integer?
    if (!isdigit(text[0]) || text[0] != L'"')
        processError(this, L"isn't constant integer", __FILE__, __func__, __LINE__);

    if (isdigit(text[0]))
    {
        ret.type = value::IMM;
        ret.imm = util::toInt(text);
    }
    else if (text[0] == '"')
    {
        ret.type = value::STR;
        ret.str = text;
    }
    return ret;
}
void parserCore::assign()
{
    struct value target = editable();
    std::wstring op = iter.next();
    struct expr value;
    if (!(op == L"++" || op == L"--"))
    {
        value = expr();
    }
    stmtProcessor::Assign(this, target, op, value);
}
struct power parserCore::power()
{
    struct power ret;
    if (iter.peekSafe() == L"(")
    {
        ret.type = power::EXPR;
        // inner type
        iter.next();
        ret.expr = expr();
        assertChar(")");
    }
    else if (isFunccall(iter.peekSafe(), iter.peekSafe(1)))
    {
        ret.type = power::FUNCCALL;
        struct ExecFunc func = funcCall();
        ExecFunc *func2 = new ExecFunc;
        func2->args = func.args;
        func2->funcAddr = func.funcAddr;
        func2->funcId = func.funcId;
        func2->type = func.type;
        ret.func = func2;
    }
    else if (isInt(iter.peekSafe()))
    {
        ret.type = power::IMM;
        ret.imm = Int();
    }
    else if (isSingle(iter.peek()) && iter.peek() != L"[")
    {
        ret.type = power::VAR;
        ret.var = iter.next();
    }
    else if (iter.peek() == L"[")
    {
        ret.type = power::PTR;
        ret.Pointer = ptr();
    }
    else
    {
        ret.type = power::EXPR;
        ret.expr = expr();
    }
    return ret;
}
struct expo parserCore::expo()
{
    struct expo val;
    val.parts.emplace_back(power());
    while (iter.hasData() && iter.peek() == L"**")
    {
        assertChar("**");
        val.parts.emplace_back(power());
    }
    return val;
}
struct term parserCore::term()
{
    struct term ret;
    {
        expo_wrap wrap;
        wrap.type = expo_wrap::MUL;
        wrap.value = expo();
        ret.parts.emplace_back(wrap);
    }
    while (
        iter.hasData() &&
        (iter.peek() == L"*" ||
         iter.peek() == L"/" ||
         iter.peek() == L"%"))
    {
        auto text = iter.next();
        assert(
            text == L"*" ||
                text == L"/" ||
                text == L"%",
            L"excepted '*' or '/','%'");

        expo_wrap Elem;
        if (text == L"*")
        {
            Elem.type = expo_wrap::MUL;
        }
        else if (text == L"/")
        {
            Elem.type = expo_wrap::DIV;
        }
        else if (text == L"%")
        {
            Elem.type = expo_wrap::MOD;
        }
        Elem.value = expo();
        ret.parts.emplace_back(Elem);
    }
    return ret;
}
struct expr parserCore::expr()
{
    struct expr ret;
    struct term part;
    std::wstring text = iter.peek();

    part = term();
    if (text == L"+")
    {
        iter.next(); // read
    }
    else if (text == L"-")
    {
        iter.next(); // read
        struct power pow;
        pow.type = power::IMM;
        pow.imm = -1;

        struct expo tmp;
        tmp.parts.emplace_back(pow);

        struct expo_wrap wrap;
        wrap.value = tmp;
        wrap.type = expo_wrap::MUL;

        part.parts.emplace_back(wrap);
    }
    ret.parts.emplace_back(part);
    while (
        iter.hasData() && (iter.peek() == L"+" ||
                           iter.peek() == L"-" ||
                           isBitOpFull(iter.peek())))
    {
        auto text = iter.next();
        assert(
            text == L"+" ||
                text == L"-" ||
                isBitOpFull(text),
            L"excepted '+' or '+', bitOperator");

        part = term();
        if (text == L"-")
        {
            struct power pow;
            pow.type = power::IMM;
            pow.imm = -1;

            struct expo tmp;
            tmp.parts.emplace_back(pow);

            struct expo_wrap wrap;
            wrap.value = tmp;
            wrap.type = expo_wrap::MUL;

            part.parts.emplace_back(wrap);
        }
        ret.parts.emplace_back(part);
    }
    return ret;
}
parserCore::Range parserCore::range()
{
    int start = Int();
    assertChar("...");
    int end = Int();
    // convert start/end: wstr => int
    return std::make_pair(start, end);
}
int parserCore::Int()
{
    std::wstring text = iter.next();
    if (!isdigit(text[0]))
    {
        syntaxError(this, L"is not integer", __FILE__, __func__, __LINE__);
    }
    // convert test<wstr> to value<int>
    return toInt(text);
}
void parserCore::If()
{
    assertChar("if");
    struct cond conditional = cond();
    stream << "# if" << std::endl;
    assertChar("{");
    stmtProcessor::If(this, conditional);
    assertChar("}");
    stream << "# fi" << std::endl;
}
struct cond parserCore::cond()
{
    struct cond ret;
    std::wstring text;
    ret.conds.emplace_back(condAnd());
    while (
        iter.hasData() &&
        (iter.peek() == L"||"))
    {
        assertChar("||");
        ret.conds.emplace_back(condAnd());
    }
    return ret;
}
struct condAnd parserCore::condAnd()
{
    struct condAnd ret;
    ret.conds.emplace_back(cond_inner());
    while (
        iter.hasData() &&
        (iter.peek() == L"&&"))
    {
        assertChar("&&");
        ret.conds.emplace_back(cond_inner());
    }
    return ret;
}
struct condChild parserCore::cond_inner()
{
    struct condChild cond;
    std::wstring text;

    std::wstring maybeOp;
    {
        auto offs = iter.index;
        value();
        maybeOp = iter.peekSafe();
        iter.index = offs;
    }
    if (isCondOpFull(maybeOp))
    {
        cond.val1 = expr();
        std::wstring op = iter.next();
        if (op == L"<")
        {
            cond.op = condChild::LT;
        }
        else if (op == L">")
        {
            cond.op = condChild::GT;
        }
        else if (op == L"<=")
        {
            cond.op = condChild::LE;
        }
        else if (op == L">=")
        {
            cond.op = condChild::GE;
        }
        else if (op == L"==")
        {
            cond.op = condChild::EQU;
        }
        else if (op == L"!=")
        {
            cond.op = condChild::NEQ;
        }
        cond.val2 = expr();
    }
    else
    {
        cond.op = condChild::SINGLE;
        cond.single = value();
    }
    return cond;
}
void parserCore::While()
{
    assertChar("while");
    struct cond conditional = cond();
    stream << "# while {" << std::endl;
    assertChar("{");
    stmtProcessor::While(this, conditional);
    assertChar("}");
    stream << "# }" << std::endl;
}
void parserCore::mcl()
{
    assertChar("mcl");
    (*wraper) << util::wstr2str(iter.next());
}
struct ExecFunc parserCore::funcCall()
{
    ExecFunc ret;
    if (iter.peek() == L"func")
    {
        // address based call
        ret.type = ExecFunc::ADDRESS;

        assert(iter.next() == L"func", L"excepted 'func'");
        assertChar("[");
        ret.funcAddr = Int();
        assertChar("]");
    }
    else
    {
        // name based call
        ret.type = ExecFunc::Name;
        ret.funcId = ident();
    }
    assertChar("(");
    if (iter.peek() != L")")
    {
        ret.args.emplace_back(expr());
    }
    while (iter.peek() != L")")
    {
        assertChar(",");
        ret.args.emplace_back(expr());
    }
    assertChar(")");
    return ret;
}

std::wstring
convPut(std::wstring src, std::wstring argument)
{
    std::wstring ret;
    auto iter = iterator<wchar_t>(util::convToVector<wchar_t, std::wstring>(src));
    while (iter.hasData())
    {
        wchar_t ch = iter.next();
        wchar_t ne = iter.peekSafe(1);
        if (ch == '$' && ne == '$')
        {
            ret += '$';
            iter.next();
        }
        else if (
            ch == '$' &&
            iter.peekSafe(0) == 'a' &&
            iter.peekSafe(1) == 'r' &&
            iter.peekSafe(2) == 'g')
        {
            for (auto ch : argument)
            {
                ret += ch;
            }
            iter.index += 3;
        }
        else
        {
            ret += ch;
        }
    }
    return ret;
}