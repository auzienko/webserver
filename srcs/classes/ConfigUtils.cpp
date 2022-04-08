#include "../../includes/classes/ConfigUtils.hpp"

ConfigUtils::~ConfigUtils(void){}

std::pair<std::string, t_location const*> ConfigUtils::getLocationSettings(
    t_server const& s, std::string const& request_path) {
  size_t len = request_path.length();
  if (!len) return std::make_pair("!__empty__path__", nullptr);
  while (--len && request_path.at(len) != '/')
    ;
  if (!len && request_path.at(len) != '/')
    return std::make_pair("!__backslash__not__found__in__path__", nullptr);
  std::string loc(request_path, 0, len + 1);
  std::string fl(request_path, len + 1, request_path.length());
  std::vector<t_location>::const_iterator i = s.locations.begin();
  std::vector<t_location>::const_iterator e = s.locations.end();
  while (i != e) {
    if ((*i).path == loc) {
      if (fl.length())
        return std::make_pair(fl, &(*i));
      else
        return std::make_pair((*i).index, &(*i));
    }
    ++i;
  }
  return std::make_pair(request_path, &(s.locations.at(0)));
}

static std::vector<std::string> trim(std::string line)
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

static bool isCGI(std::vector<std::string> dirs, std::map<std::string, std::string> const& cgi, t_uriInfo &res)
{
  std::map<std::string, std::string>::const_iterator  it;
  std::map<std::string, std::string>::const_iterator  end = cgi.cend();
  size_t  pPos;
  std::string file(dirs.back());
  std::string pathInfo;

  while (file.find('.') != std::string::npos)
  {
    pathInfo = '/' + file + pathInfo;
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
      return (true);
    }
  }
  return (false);
  // for (size_t i = 0; !isMiss && !isPass; ++i)
  // {
  //   isMiss = true;
  //   for (it = cgi.cbegin(); !isPass && it != end; ++it)
  //   {
  //     if (it->first == path)
  //       isPass = true;
  //     else if (it->first.find(path) != std::string::npos)
  //       isMiss = false;
  //   }
  //   if (dirs.empty() || isPass)
  //     break;
  //   if (path == ".")
  //     path = dirs.front();
  //   else
  //     path = path + "/" + dirs.front();
  //   dirs.erase(dirs.begin());
  // }
  // if (isPass)
  // {
  //   res.uri = (--it)->second;
  //   res.loc = NULL;
  //   res.isCgi = true;
  //   while (!dirs.empty())
  //   {
  //     res.pathInfo = res.pathInfo + "/" + dirs.front();
  //     dirs.erase(dirs.begin());
  //   }
  // }
}

static void fillUri(std::string const &line, t_server const& server_config, t_uriInfo &res)
{
  bool  isFind = false;
  std::vector<t_location>::const_iterator it;
  std::vector<t_location>::const_iterator end = server_config.locations.cend();
  std::string checkLine;
  std::vector<std::string> dirs = trim(line);
  size_t  max_i = dirs.size();

  isFind = isCGI(dirs, server_config.cgi, res);
  for (size_t i = 0; !isFind && max_i >= i; ++i)
  {
    checkLine = setLine(dirs, i);
    it = server_config.locations.cbegin();
    while (!isFind && it != end)
    {
      if (checkLine == it->path)
      {
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
      }
      ++it;
    }
  }
}

t_uriInfo ConfigUtils::parseURI(std::string &uri, t_server const& server_config){
  t_uriInfo res;

  res.isCgi = false;
  std::string url = (uri.find('%') != std::string::npos ? ws::stringUrlDecode(uri) : uri);
  size_t  delim = url.find('?');

  if (delim == std::string::npos)
    fillUri(url, server_config, res);
  else
  {
    fillUri(url.substr(0, delim), server_config, res);
    res.args = url.substr(delim + 1);
  }
  return (res);
}


// my/path/file.html
// my/path/file.html?var=n&var2=n
// my/p%eeth/file.html
// my/path/cgi
// my/path/cgi/a/b
// my/path/cgi/a/b?var=n
