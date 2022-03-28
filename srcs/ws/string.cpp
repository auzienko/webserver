#include "../../includes/main.hpp"

std::string ws::stringTail(std::string const& str, char c) {
  size_t len = str.length() - 1;
  std::string tmp;
  while (len && str[len] != c) {
    tmp += c;
    --len;
  }
  return ws::stringReverse(tmp);
}

std::string ws::stringReverse(std::string& str) {
  int n = str.length();
  for (int i = 0; i < n / 2; i++) std::swap(str[i], str[n - i - 1]);
  return (str);
}

std::string ws::stringUrlDecode(std::string const& str) {
  std::string result;
  char ch;
  size_t len = str.length();
  int ii;
  size_t max = str.find('?');

  for (size_t i = 0; i < len && (i < max || max == std::string::npos); ++i) {
    if (str[i] != '%') {
      if (str[i] == '+')
        result += ' ';
      else
        result += str[i];
    } else {
      sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
      ch = static_cast<char>(ii);
      result += ch;
      i = i + 2;
    }
  }
  return result;
}
