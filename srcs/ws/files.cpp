#include "main.hpp"
#include "classes/MimeTypes.hpp"

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

std::pair<std::ifstream, std::string>* ws::filesErrors(int code, const std::map<int, std::string>* error_pages)
{
  std::string path;
  bool isNotFind = false;
  std::pair<std::ifstream, std::string> *res = new std::pair<std::ifstream, std::string>();
  
  if (error_pages->find(code) != error_pages->end())
    path = error_pages->at(code);
  else
    return res;

  struct stat  file;
  if (stat(path.c_str(), &file)) {
    isNotFind = true;
  }
  if (!isNotFind && S_ISDIR(file.st_mode)) {
    isNotFind = true;
  } else {
    res->first.open(path, std::ifstream::binary);
    if (!res->first.is_open()) {
      isNotFind = true;
    } else
      res->second = MimeTypes::getMimeType(path);
  }
  return res;
}
