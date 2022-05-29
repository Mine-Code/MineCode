#include <iostream>
#include <tokenizer/tokenizer.hpp>
#include <fstream>

int main(int, char**) {
  using namespace minecode::tokenizer;

  std::ifstream ifs("demo/mc2.mc");

  Tokenizer<char> tokenizer{
      "int main(int argc, char **argv) {\n"
      "  return 0;\n"
      "}\n"};
  auto tokens = tokenizer.Tokenize();

  for (auto token : tokens) {
    std::cout << token->ToString() << std::endl;
  }
}
