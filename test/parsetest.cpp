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
        L"#do once\n"
        "func sendMessagesToPlayers()\n"
        "{\n"
        "  for player in ServerPlayer.List.name {\n"
        "    flag = [[0x10000000] + 0o20] + 0xff + 1;\n"
        "    [flag]++;\n"
        "    player.Chat << f\"{player.name}さん、こんにちは！ \\\" hoge \\\"\";\n"
        "  }\n"
        "}"s
    );
    prs.tokenize();
    prs.parse();
    prs.debug();
    return 0;
}
