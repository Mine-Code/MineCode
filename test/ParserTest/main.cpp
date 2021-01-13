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
    prs.debug();
    
    prs.set(
        L""
        "#do once"                          "\n"
        "func test(){"                      "\n"
        //"  a = 1+2+3"                       "\n"
        //"  n = (a*(100+a*10)+3*(1+5))/20+a" "\n" 
        "  if 0o20==0x10{"                  "\n"
        "    Debug << \"^^\" "              "\n"
        "  }"                               "\n"
        //"  for i in 0...0x10{"              "\n"
        //"    Debug << \"{i}\" "     "\n"
        //"  }"                               "\n"
        //"  while a<=10{"                    "\n"
        //"    Debug << \"(^^)v\""    "\n"
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

    optimizer a = optimizer(compiled);
    a.get();

    //prs.debug();
    return 0;
}