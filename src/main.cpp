#include <parserWrap.h>
#include <mcl.h>
#include <optimizer.h>

#include <fstream>

using namespace std::literals::string_literals;

int main(int argc, char *argv[])
{
	parserWrap prs;
	std::ifstream file;

	//argument check
	if(argc!=2){
		std::wcout
			<<"Error: Invalid argument count"<<std::endl
			<<"[Usage] "<<argv[0]<<" <Source filename>"<<std::endl;
	}

	return 0;
}