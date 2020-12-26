#include <parserWrap.h>
#include <ios>
#include <iostream>

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
    prs.set(
        L"#do once"                      "\n"
        "func test(){"                   "\n"
        "  a = 2;"
        "  n = (a*(100+a*10)+3*(1+a))/20+a" "\n" 
        "  if(0o20==0x10){"               "\n"
        "    player.Chat << \"^^\" "     "\n"
        "  }"                            "\n"
        "  for(i in 0...0x10){"           "\n"
        "    player.Chat << \"{i}\" "    "\n"
        "  }"                            "\n"
        "}"s
    );
    prs.tokenize();
    prs.parse();
    prs.debug();
    return 0;
}
