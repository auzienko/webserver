#include "../../includes/main.hpp"

bool ws::uriIsCGI(std::vector<std::string> dirs, std::map<std::string, std::string> const& cgi, t_uriInfo &res)
{
  std::map<std::string, std::string>::const_iterator  it;
  std::map<std::string, std::string>::const_iterator  end = cgi.cend();
  size_t  pPos;
  std::string file(dirs.back());
  std::string pathInfo;

  while (file.find('.') == std::string::npos && !dirs.empty())
  {
    pathInfo = '/' + file + pathInfo;
    dirs.pop_back();
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

void ws::uriFill(std::string const &line, t_server const& server_config, t_uriInfo &res, std::string lPath)
{
  bool  isFind = false;
  std::vector<t_location>::const_iterator it;
  std::vector<t_location>::const_iterator end = server_config.locations.cend();
  std::string checkLine;
  std::vector<std::string> dirs = ws::stringTrim(line);
  size_t  max_i = dirs.size();

  isFind = ws::uriIsCGI(dirs, server_config.cgi, res);
  for (size_t i = 0; !isFind && max_i >= i; ++i)
  {
    checkLine = setLine(dirs, i);
    it = server_config.locations.cbegin();
    while (!isFind && it != end)
    {
      if (checkLine == it->path || checkLine + '/' == it->path)
      {
		if (checkLine == lPath)
			throw std::logic_error("Recursive redirect found (" + lPath + ")");
        isFind = true;
        res.loc = it.base();
		if (!res.loc->redir.empty())
		{
			t_uriInfo	tRes;
			std::string	tLine(res.loc->redir);
			std::vector<std::string>::iterator	end = dirs.end();
			std::vector<std::string>::iterator it = dirs.begin();

			for (size_t j = dirs.size() - i; j > 0; j--)
				++it;
			if (it != end) {
				for ( ; it != end; it++) {
					tLine += *it + "/";
				}
				tLine.pop_back();
			}
			uriFill(tLine, server_config, tRes, checkLine);
			res = tRes;
			std::cout << res.loc->path << std::endl;
			return ;
		}
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
