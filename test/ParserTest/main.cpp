#include <parserWrap.h>
#include <mcl.h>
#include <ios>
#include <iostream>
#include <optimizer.h>

using namespace std::literals::string_literals;
int main()
{
    std::ios_base::sync_with_stdio(false);
    std::wcout.imbue(
        std::locale(
            std::locale::classic(),
            std::locale(""),
            std::locale::ctype
        )
    );

    parserWrap prs;
    prs<<"debug";
    
    prs.set(
        L""
        "#do once"  "\n"
        "a = 0"     "\n"
        "Debug << \"\"" "\n"
        "a++\n"
        "Debug << \"\"" "\n"
        "a += 10"   "\n"
        "a -= 10"   "\n"
        "a *= 10"   "\n"
        "a /= 10"   "\n"
        "a %= 10"   "\n"
        "a >>= 10"  "\n"
        "a <<= 10"  "\n"
    );
    prs.tokenize();
    auto compiled=prs.compile();
    prs.debug();
    std::wcout<<compiled<<std::endl; // show compiled code
    
    // count  lines
    int lines=0;
    for(auto ch:compiled) if (ch=='\n') lines++;
    std::wcout<<"lines: "<<lines<<std::endl;

    optimizer a = optimizer(compiled);
    a.get();

    //prs.debug();
    return 0;
}