#include <parserWrap.h>
#include <mcl.h>
#include <optimizer.h>
#include "../lib/Assembler/include/asm.h"
#include <fstream>

using namespace std::literals::string_literals;

int main(int argc, char *argv[])
{
	parserWrap prs;
	std::wifstream file;

	//argument check
	if (argc < 2)
	{
		std::wcout
			<< "Error: Invalid argument count" << std::endl
			<< "[Usage] " << argv[0] << " [ -J | -h ] <Source filename>" << std::endl;
		return 1;
	}

	// read the file
	file.open(argv[1]);
	std::istreambuf_iterator<wchar_t> it(file);
	std::istreambuf_iterator<wchar_t> last;
	std::wstring str(it, last);

	// Compile!!!
	Assembler assembler;
	auto insts = assembler.PPC(prs.compile_full(str));
	bool flag = false;
	std::wcout << std::hex << std::uppercase;

	if (true) // cafecode mode
	{
		auto size = insts.size();
		std::wcout << "C000";
		std::wcout.fill('0');
		std::wcout.width(4);
		if (size & 1 == 0)
		{
			std::wcout << insts.size() / 2;
		}
		else
		{
			std::wcout << (insts.size() + 1) / 2;
		}
		std::wcout << " 60000000" << std::endl;
	}
	std::wcout.width(8);
	for (auto inst : insts)
	{
		std::wcout << inst;
		std::wcout << (flag ? '\n' : ' ');
		flag ^= 1;
	}
	if (flag)
	{
		std::wcout << "60000000" << std::endl;
	}
	return 0;
}