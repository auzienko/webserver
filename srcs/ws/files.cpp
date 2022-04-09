#include "../../includes/main.hpp"

std::pair<bool, std::vector<std::string> > ws::filesReadDirectory(
    std::string const& path) {
  std::vector<std::string> result;
  dirent* de;
  DIR* dp;
  if (path.empty()) return std::make_pair(false, result);
  dp = opendir(path.c_str());
  if (dp) {
    while (true) {
      de = readdir(dp);
      if (de == NULL) break;
      result.push_back(std::string(de->d_name));
    }
    closedir(dp);
    std::sort(result.begin(), result.end());
  } else
    return std::make_pair(false, result);
  return std::make_pair(true, result);
}

bool ws::filesIsDir(std::string const &file) {
  struct stat  buf;

  if (stat(file.c_str(), &buf))
    return (false);
  if (S_ISDIR(buf.st_mode))
    return (true);
  return (false);
}
