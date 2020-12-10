#include <parser.h>
using namespace std::literals::string_literals;
int main()
{
    parser prs;
    prs.set(
        L"func sendMessagesToPlayers()\n"
        "{\n"
        "  for player in ServerPlayer.List.name {\n"
        "    flag = [[0x10000000] + 0o20] + 0xff + 1;\n"
        "    [flag]++;\n"
        "    player.Chat << f\"{player.name}さん、こんにちは！ \\\" hoge \\\"\";\n"
        "  }\n"
        "}"s
    );
    prs.tokenize();
    prs.debug();
    return 0;
}
