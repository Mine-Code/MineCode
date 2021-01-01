#include <mcl.h>

#include <fstream>

mcl::mcl(std::wstring name){
    std::ifstream file("file.json");
    file >> raw;
}