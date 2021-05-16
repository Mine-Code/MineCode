#include <syntaxError.h>
#include <parserTypes.h>

using namespace synErr;

void synErr::ReportError(parserTypes::parserContext &ctx, errorType type, std::wstring exMsg, const char *fname, const char *funcname, int line)
{
    std::wstring strType;
    if (type == SYNTAX)
    {
        strType = L"syntax";
    }
    else if (type == TOKENIZE)
    {
        strType = L"tokenize";
    }
    else if (type == PROCESS)
    {
        strType = L"process";
    }

    std::wstring prev = ctx.iter.peekSafe(-1);
    std::wstring curr = ctx.iter.peekSafe(0);
    std::wstring next = ctx.iter.peekSafe(1);
    std::wstring pos = prev + curr + next;

    std::wstring offset = std::wstring(prev.length(), L' ');
    std::wstring uline = std::wstring(curr.length(), L'^');
    std::wcout
        << "-------Error--------" << std::endl
        << "| type:" << strType << std::endl;
    if (exMsg != L"")
        std::wcout
            << "| msg :" << exMsg << std::endl;
    std::wcout
        << "| Where?" << std::endl
        << "|  func: " << funcname << std::endl
        << "|  copyable: " << fname << ":" << line << std::endl
        << "| Pos Informations" << std::endl
        << "| " << pos << std::endl
        << "| " << offset << uline << std::endl;
    std::wcout
        << "--------------------" << std::endl;

    throw exMsg;
}
void synErr::syntaxError(parserTypes::parserContext &ctx, std::wstring ex, const char *fname, const char *funcname, int line) { ReportError(ctx, SYNTAX, ex, fname, funcname, line); }
void synErr::tokenizeError(parserTypes::parserContext &ctx, std::wstring ex, const char *fname, const char *funcname, int line) { ReportError(ctx, TOKENIZE, ex, fname, funcname, line); }
void synErr::processError(parserTypes::parserContext &ctx, std::wstring ex, const char *fname, const char *funcname, int line) { ReportError(ctx, PROCESS, ex, fname, funcname, line); }