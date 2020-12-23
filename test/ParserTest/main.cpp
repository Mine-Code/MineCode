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
        "  n = (2*(100+20)+3*(1+2))/20+1" "\n" 
        "  if 0o20==0x10{"               "\n"
        "    player.Chat << \"^^\" "     "\n"
        "  }"                            "\n"
        "  for i in 0...0x10{"           "\n"
        "    player.Chat << \"{i}\" "    "\n"
        "  }"                            "\n"
        "}"s
    );
    prs.tokenize();
    prs.parse();
    prs.debug();
    return 0;
}
