#include <parserWrap.h>
#include <mcl.h>
#include <optimizer.h>

#include <fstream>

using namespace std::literals::string_literals;

int main(int argc, char *argv[])
{
	parserWrap prs;
	std::wifstream file;

	//argument check
	if (argc != 2)
	{
		std::wcout
			<< "Error: Invalid argument count" << std::endl
			<< "[Usage] " << argv[0] << " <Source filename>" << std::endl;
		return 1;
	}

	// read the file
	file.open(argv[1]);
	std::istreambuf_iterator<wchar_t> it(file);
	std::istreambuf_iterator<wchar_t> last;
	std::wstring str(it, last);

	// Compile!!!
	std::wcout << prs.compile_full(str) << std::endl;
	return 0;
}