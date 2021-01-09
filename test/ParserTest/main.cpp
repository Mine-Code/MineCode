#include <parserWrap.h>
#include <mcl.h>
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
    prs.ctx<<"minecraft";
    prs.set(
        L""
        "#do once"                          "\n"
        "func test(){"                      "\n"
        "  a = 2"                           "\n"
        //"  n = (a*(100+a*10)+3*(1+5))/20+a" "\n" 
        //"  if 0o20==0x10{"                  "\n"
        //"    Local.Console << \"^^\" "      "\n"
        //"  }"                               "\n"
        //"  for i in 0...0x10{"              "\n"
        //"    Local.Console << \"{i}\" "     "\n"
        //"  }"                               "\n"
        //"  while a<=10{"                    "\n"
        //"    Local.Console << \"(^^)v\""    "\n"
        //"  }"                               "\n"
        "}"s
    );
    prs.tokenize();
    auto compiled=prs.compile();
    std::wcout<<compiled<<std::endl; // show compiled code
    
    // count  lines
    int lines=0;
    for(auto ch:compiled) if (ch=='\n') lines++;
    std::wcout<<"lines: "<<lines<<std::endl;
    prs.debug();
    return 0;
}