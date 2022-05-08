#include "main.hpp"

std::string ws::stringTail(std::string const& str, char c) {
  size_t len = str.length() - 1;
  std::string tmp;
  while (len && str[len] != c) {
    tmp += str[len];
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

std::string ws::stringFromMap(std::map<std::string, std::string>::const_iterator it,
    std::map<std::string, std::string>::const_iterator end)
{
  if (end == it)
    return ("");
  return (it->second);
}

std::string ws::stringToCGIFormat(std::string const& str){
    std::string result = str;
    std::replace(result.begin(), result.end(), '-', '_');
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::vector<std::string> ws::stringTrim(std::string line)
{
  std::vector<std::string> res;
  size_t  delim = line.find('/', 1);

  while (delim != std::string::npos)
  {
    res.push_back(line.substr(1, delim - 1));
    line.erase(0, delim);
    delim = line.find('/', 1);
  }
  res.push_back(line.substr(1, delim - 1));
  return (res);
}

void	ws::stringSkipWS(std::string &line)
{
	const char	*ptr = line.c_str();
	std::string	spaces = WHITE_SPACES;

	while (*ptr && spaces.find(*ptr) != std::string::npos)
	{
		ptr++;
	}
	line = ptr;
}

int ws::stringTakeErrCode(const std::string &msg)
{
  int code = atoi(msg.c_str());
  if (code == 0)
    return -1;
  return code;
}
