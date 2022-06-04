#pragma once

#include <sstream>

namespace minecode::preprocessor {
template <typename CharT>
class PreProcessor {
 private:
  using String = std::basic_string<CharT>;
  std::istringstream stream;

 public:
  PreProcessor(const String &src) : stream(src) {}

  String PreProcess() {
    String ret;
    while (stream.good()) {
      auto line = ReadLine();
      if (line.size() > 0) {
        ret += line + "\n";
      }
    }
    return ret;
  }

 private:
  String _ReadLine() {
    String line;
    while (stream.good()) {
      auto c = stream.get();
      if (c == '\n') {
        break;
      }
      line += c;
    }
    return line;
  }
  String ReadLine() {
    do {
      auto line = this->_ReadLine();
      if (line.empty()) {
        continue;
      }
      if (line[0] == '#') {
        continue;
      }
      return line;
    } while (stream.good());
    return "";
  }
};
}  // namespace minecode::preprocessor