//===------------------------------------------------------------*- C++ -*-===//
///
/// \file
/// \brief Defines some string manipulation functions
///
//===----------------------------------------------------------------------===//
#ifndef STRING_H
#define STRING_H

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace Common {
// Characters to treat as whitespace
const static char *WhiteSpace = " \t\n\r\f\v";
// Trim from left side / start
inline std::string &ltrim(std::string &s, const char *ws = WhiteSpace) {
  s.erase(0, s.find_first_not_of(ws));
  return s;
}

// Trim from right side / end
inline std::string &rtrim(std::string &s, const char *ws = WhiteSpace) {
  s.erase(s.find_last_not_of(ws) + 1);
  return s;
}

// Trim from both ends
inline std::string &trim(std::string &s, const char *ws = WhiteSpace) {
  return ltrim(rtrim(s, ws), ws);
}

// Split based on delimiter
inline std::vector<std::string> split(const std::string &s, char delim = ' ') {
  std::vector<std::string> tokens;
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    tokens.push_back(item);
  }
  return tokens;
}
} // namespace Common
#endif
