#include <mcl.h>
#include <optimizer.h>
#include <parserWrap.h>
#include <tinyasm.h>

#include <fstream>

using namespace std::literals::string_literals;

int main(int argc, char *argv[]) {
  parserWrap prs;
  std::wifstream file;

  // argument check
  if (argc < 2) {
    std::wcout << "Error: Invalid argument count" << std::endl
               << "[Usage] " << argv[0] << " [ -J | -h ] <Source filename>"
               << std::endl;
    return 1;
  }

  // analysis argument(s)
  bool isCafecode = false;
  bool isAssembly = false;
  bool hasSrcFile = false;
  std::string srcfile = "";
  for (int i = 1; i < argc; i++) {
    std::string arg(argv[i]);
    if (arg == "-h") {
      std::wcout << "[Usage] " << argv[0]
                 << " [ -J | -h | -c ] <Source filename>" << std::endl
                 << "" << std::endl
                 << "Argument Options" << std::endl
                 << "----------" << std::endl
                 << "-J    | Output CafeCode" << std::endl
                 << "-c    | Output Assembler code(compile only)"
                 << "-h    | Show help (this message)" << std::endl;
      std::exit(0);
    } else if (arg == "-J") {
      if (isAssembly) {
        std::wcout << "Error: cannot set Cafecode mode when compile only mode";
        std::exit(3);
      }
      isCafecode = true;
    } else if (arg == "-c") {
      if (isCafecode) {
        std::wcout << "Error: cannot set compile only mode when Cafecode mode";
        std::exit(3);
      }
      isAssembly = true;
    } else {
      if (hasSrcFile) {
        std::wcout << "Error: Too many input files (require one source file)"
                   << std::endl;
        std::exit(2);
      }
      srcfile = arg;
      hasSrcFile = true;
    }
  }
  if (!hasSrcFile) {
    std::wcout << "Error: No input files" << std::endl;
    std::exit(1);
  }
  // read the file
  file.open(srcfile);
  std::istreambuf_iterator<wchar_t> it(file);
  std::istreambuf_iterator<wchar_t> last;
  std::wstring str(it, last);

  // Compile!!!
  if (isAssembly) {
    std::wcout << prs.compile_full(str) << std::endl;
  } else {
    Assembler assembler;
    auto insts = assembler.PPC(prs.compile_full(str));
    bool flag = false;
    std::wcout << std::hex << std::uppercase;

    if (isCafecode)  // cafecode mode
    {
      auto size = insts.size();
      std::wcout << "C000";
      std::wcout.fill('0');
      std::wcout.width(4);
      if (size % 2 == 1) {
        std::wcout << insts.size() / 2;
      } else {
        std::wcout << (insts.size() + 1) / 2;
      }
      std::wcout << " 60000000" << std::endl;
    }
    std::wcout.width(8);
    for (auto inst : insts) {
      std::wcout.fill('0');
      std::wcout.width(8);
      std::wcout << inst;
      std::wcout << (flag ? '\n' : ' ');
      flag ^= 1;
    }
    if (flag) {
      std::wcout << "60000000" << std::endl;
    }
  }
  return 0;
}