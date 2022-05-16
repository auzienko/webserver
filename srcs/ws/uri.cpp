#include "../../includes/main.hpp"

bool ws::uriIsCGI(std::vector<std::string> dirs, std::map<std::string, std::string> const& cgi, t_uriInfo &res, std::map<std::string, std::set<std::string> > cgi_methods)
{
  std::map<std::string, std::string>::const_iterator  it;
  std::map<std::string, std::string>::const_iterator  end = cgi.cend();
  size_t  pPos;
  std::string file;
  std::string pathInfo;

  if (dirs.empty() || dirs.back().empty())
    return (false);
  file = dirs.back();
  while (!dirs.empty() && file.find('.') == std::string::npos)
  {
    pathInfo = '/' + file + pathInfo;
    dirs.pop_back();
    if (!dirs.empty())
      file = dirs.back();
  }
  if ((pPos = file.find('.')) != std::string::npos)
  {
    file.erase(0, pPos);
    if ((it = cgi.find(file)) != end)
    {
      res.uri = it->second;
      res.loc = NULL;
      res.isCgi = true;
      res.pathInfo = pathInfo;
      res.cgi_methods = cgi_methods.find(file)->second;
      return (true);
    }
  }
  return (false);
}

static std::string  setLine(std::vector<std::string> dirs, size_t nLast)
{
  std::string res("/");
  nLast++;
  while (dirs.size() > nLast)
  {
    res = res + dirs.front() + "/";
    dirs.erase(dirs.begin());
  }
  if (dirs.size() > nLast - 1)
    res = res + dirs.front();
  return (res);
}

void ws::uriFill(std::string const &line, t_server const& server_config, t_uriInfo &res, std::string method)
{
  bool  isFind = false;
  bool  isMethodErr = false;
  std::vector<t_location>::const_iterator it;
  std::vector<t_location>::const_iterator end = server_config.locations.cend();
  std::string checkLine;
  std::vector<std::string> dirs = ws::stringTrim(line);
  size_t  max_i = dirs.size();

  isFind = ws::uriIsCGI(dirs, server_config.cgi, res, server_config.cgi_methods);
  for (size_t i = 0; !isFind && max_i >= i; ++i)
  {
    checkLine = setLine(dirs, i);
    it = server_config.locations.cbegin();
    while (!isFind && it != end)
    {
      if ((checkLine == it->path || checkLine + '/' == it->path)) {
        if ((!it->methods.size() || it->methods.find(method) != it->methods.end())) {
          isFind = true;
          res.loc = it.base();
          res.uri = it->root;
          while (dirs.size() > i)
            dirs.erase(dirs.begin());
          while (!dirs.empty())
          {
            res.uri = res.uri + dirs.front() + (dirs.size() > 1 ? "/" : "");
            dirs.erase(dirs.begin());
          }
        } else {
          isMethodErr = true;
        }
      }
      ++it;
    }
  }
  if (!isFind && isMethodErr)
    throw std::logic_error(method == "HEAD" ? "405h" : "405");
}
