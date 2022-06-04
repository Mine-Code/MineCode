#include <iostream>
#include <tokenizer/tokenizer.hpp>
#include <preprocessor/preprocessor.hpp>
#include <fstream>

int main(int, char**) {
  using namespace minecode::tokenizer;
  using namespace minecode::preprocessor;

  std::ifstream ifs("demo/mc2.mc");
  std::string src((std::istreambuf_iterator<char>(ifs)),
                  (std::istreambuf_iterator<char>()));

  PreProcessor<char> pp(src);
  std::string preprocessed = pp.PreProcess();

  Tokenizer<char> tk(preprocessed);
  auto tokens = tk.Tokenize();

  for (auto token : tokens) {
    std::cout << token->ValueToString() << " ";
  }
}
