#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <tinyutil.h>

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace util {
int _toInt(std::wstring);

template <typename T>
bool isMathOp(T ch) {
  return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%';
}

template <typename T>
bool isBitOp(T ch) {
  return ch == '|' || ch == '^' || ch == '&';
}

template <typename T>
bool isCondOp(T ch) {
  return ch == '<' || ch == '>' || ch == '=' || ch == '!';
}

template <typename T>
bool isCondOpFull(T str) {
  return str == L"<" || str == L">" || str == L"<=" || str == L">=" ||
         str == L"==" || str == L"!=";
}
template <typename T>
bool isBitOpFull(T str) {
  return str == L"|" || str == L"^" || str == L"&" || str == L">>" ||
         str == L"<<";
}

template <typename T>
bool isIdentity(T ch) {
  return ch == '{' || ch == '}' || ch == '(' || ch == ')' || ch == '[' ||
         ch == ']'

         || isMathOp(ch)

         || isBitOp(ch)

         || isCondOp(ch)

         || ch == ';' || ch == '.';
}

template <typename T>
bool isAssignOp(T str) {
  if (str == L"=") {
    return true;
  }
  if (str == L"++" || str == L"--") {
    return true;
  }
  if (str.back() != '=') {
    return false;
  }
  if (str.size() == 1) {
    if (str[0] != '=') {
      return false;
    }
    return true;
  } else if (str.size() == 2) {
    char start = str[0];
    if (isMathOp(start) || isBitOp(start)) {
      return true;
    }
  } else if (str.size() == 3) {
    char start = str[0];
    char second = str[1];
    if (start != second) {
      return false;
    }
    if (start == '<' || start == '>') {
      return true;
    }
  }

  return false;
}

template <typename T, typename T2>
std::vector<T> convToVector(T2 str) {
  return std::vector<T>(str.begin(), str.end());
}

template <typename T>
bool isOp(T val) {
  return isBitOp(val) || isCondOp(val) || isMathOp(val);
}

template <typename T>
int toInt(T _text) {
  return _toInt(std::wstring(_text));
}

template <typename T>
bool isimm(T text) {
  int termCounter = 0;
  for (auto ch : text) {
    if (ch == '+' || ch == '-') {
      termCounter += 1;
    } else if (!isdigit(ch)) {
      return false;
    }
  }
  return termCounter == 1;
}

template <typename T>
bool isSingle(T text) {
  int counter = 0;
  for (auto ch : text) {
    if (ch == '+' || ch == '-') {
      counter += 1;
    }
  }
  return counter == 0;
}

template <typename T>
bool isInt(T text) {
  bool skip = (text[0] == '0' && (text[1] == 'x' || text[1] == 'o'));
  int counter = 0;
  for (auto ch : text) {
    if (isdigit(ch)) {
    } else if (text[0] == '0' && text[1] == 'x' && isHex(ch)) {
    } else {
      if (skip && counter < 2) {
        continue;
      }
      return false;
    }
    counter++;
  }
  return true;
}

template <typename T>
bool isFunccall(T text, T next) {
  return text == L"func" && next == L"[" || next == L"(";
}

template <typename T>
bool isVariable(T text) {
  int counter = 0;
  for (auto ch : text) {
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
      counter += 1;
    }
  }
  return counter == 0;
}

template <typename T>
bool isIdent(T ch) {
  return isalpha(ch) || ch == '_' || isDec(ch) || ch == '$';
}

std::string wstr2str(std::wstring str);
std::wstring str2wstr(std::string str);
template <typename KeyT, typename ValT>
bool in(std::unordered_map<KeyT, ValT> map, KeyT key) {
  return map.count(key) == 1;
}
template <typename ValT>
bool in(std::vector<ValT> vec, ValT val) {
  for (auto&& elem : vec) {
    if (elem == val) {
      return true;
    }
  }
  return false;
}
}  // namespace util

#endif