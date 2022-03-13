#include "../../includes/classes/Config.hpp"

Config::Config(std::string const& file_name) : _configFile(file_name), _debugLine(0)
{
	_fileStream.open(_configFile);
}

Config::~Config(void) {}

static void	skipWS(std::string &line)
{
	const char	*ptr = line.c_str();
	std::string	spaces = WHITE_SPACES;

	while (*ptr && spaces.find(*ptr) != std::string::npos)
	{
		ptr++;
	}
	line = ptr;
}

static void	checkisin(std::string &line, bool &isIn, int debugLine)
{
	if (line[0] == '{')
	{
		isIn = true;
		line.erase(0, 1);
		skipWS(line);
		if (!line.length() || line[0] == '#')
			return ; 
		throw std::logic_error("Find unsuspected char in config file line " + std::to_string(debugLine));   //debug mode (+)
	}
}

void	Config::_setDefaultServer()
{
	_server.cgi.clear();
	_server.client_max_body_size = 0;
	_server.error_pages.clear();
	_server.listen.clear();
	_server.locations.clear();
	_server.port = 0;
	_server.server_name.clear();
}

void	Config::_serverNameLine(std::string &line, bool &is_server, bool &is_inserver)
{
	is_server = true;
	line = line.c_str() + 6;
	skipWS(line);
	if (!line.length() || line[0] == '#' || line[0] == '{')
		return ;
	const char	*ptr = line.c_str();
	std::string	spaces = WHITE_SPACES "#{";
	size_t		len = 0;
	while (*ptr && spaces.find(*ptr) == std::string::npos)
	{
		len++;
		ptr++;
	}
	_server.server_name = line.substr(0, len);
	line = line.c_str() + len;
	skipWS(line);
	if (!line.length() || line[0] == '#')
		return ;
	checkisin(line, is_inserver, _debugLine);
	if (!line.length() || line[0] == '#')
		return ;
	throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));   //debug mode (+)
}

static void	savePath(std::string &line, std::string &field, int debugLine)
{
	size_t		len = 0;
	std::string	spaces = WHITE_SPACES "#";

	while (line[len] && spaces.find(line[len]) == std::string::npos)
		len++;
	field = line.substr(0, len);
	line = line.c_str() + len;
	skipWS(line);
	if (line.length() && line[0] != '#')
		throw std::logic_error("Find unsuspected char in config file line " + std::to_string(debugLine));   //debug mode (+)
}

void	Config::_locationNameLine(std::string &line, bool &is_location, bool &is_inlocation)
{
	t_location	cur;
	size_t		len = 0;
	std::string	spaces = WHITE_SPACES "#";

	is_location = true;
	line = line.c_str() + 8;
	skipWS(line);
	if (!line.length() || line[0] == '#' || line[0] == '{')
	{
		throw std::logic_error("There are no argument when it's need in config file line " + std::to_string(_debugLine));
	}
	while (line[len] && spaces.find(line[len]) == std::string::npos)
		len++;
	cur.path = line.substr(0, len);
	line = line.c_str() + len;
	skipWS(line);
	_server.locations.push_back(cur);
	if (!line.length() || line[0] == '#')
		return ;
	checkisin(line, is_inlocation, _debugLine);
	if (!line.length() || line[0] == '#')
		return ;
	throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));   //debug mode (+)
}

void	Config::_locationArgs(std::string &line)
{
	t_location	&cur = _server.locations.front();
	if (line.substr(0, 7) == "methods")
	{
		line = line.c_str() + 7;
		skipWS(line);
		while (line.length() && line[0] != '#')
		{
			if (line.substr(0, 3) == "GET")
			{
				cur.methods.get = true;
				line = line.c_str() + 3;
			}
			if (line.substr(0, 6) == "DELETE")
			{
				cur.methods.del = true;
				line = line.c_str() + 6;
			}
			if (line.substr(0, 4) == "POST")
			{
				cur.methods.post = true;
				line = line.c_str() + 4;
			}
			else
			{
				size_t	len = 0;
				std::string	spaces = WHITE_SPACES "#";
				while (line[len] && spaces.find(line[len]) == std::string::npos)
					len++;
				line = line.c_str() + len;
			}
			skipWS(line);
		}
	}
	else if (line.substr(0, 4) == "root")
	{
		line = line.c_str() + 4;
		skipWS(line);
		savePath(line, cur.root, _debugLine);
	}
	else if (line.substr(0, 9) == "autoindex")
	{
		line = line.c_str() + 9;
		skipWS(line);
		if (line.substr(0, 2) == "on")
		{
			cur.autoindex = true;
			line = line.c_str() + 2;
		}
		else if (line.substr(0, 3) == "off")
		{
			cur.autoindex = false;
			line = line.c_str() + 3;
		}
		skipWS(line);
		if (line.length() && line[0] != '#')
			throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));   //debug mode (+)
	}
	else if (line.substr(0, 5) == "index")
	{
		line = line.c_str() + 5;
		skipWS(line);
		savePath(line, cur.index, _debugLine);
	}
	else if (line.substr(0, 6) == "upload")
	{
		line = line.c_str() + 6;
		skipWS(line);
		savePath(line, cur.uploads_path, _debugLine);
	}
	else
		throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));   //debug mode (+)
}

void	Config::_serverArgs(std::string &line, bool &is_location, bool &is_inlocation)
{
	if (!_server.port)
	{
		size_t		len = 0;
		std::string	spaces = WHITE_SPACES "#";

		while (line[len] && line[len] != ':')
			len++;
		_server.listen = line.substr(0, len);
		line = line.c_str() + len + 1;
		len = 0;

		while (line[len] && spaces.find(line[len]) == std::string::npos)
			len++;
		_server.port = atoi(line.substr(0, len).c_str());
		line = line.c_str() + len;
		skipWS(line);
		if (line.length() && line[0] != '#')
			throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));   //debug mode (+)
	}
	else if (line.substr(0, 10) == "error_page")
	{
		size_t						len = 0;
		std::string					spaces = WHITE_SPACES "#";
		std::pair<int, std::string>	error_page;

		line = line.c_str() + 10;
		skipWS(line);
		while (line[len] && spaces.find(line[len]) == std::string::npos)
			len++;
		error_page.first = atoi(line.substr(0, len).c_str());
		line = line.c_str() + len;
		skipWS(line);
		if (!line.length() || line[0] == '#')
			throw std::logic_error("End of line in config file when waiting an argument line " + std::to_string(_debugLine));
		savePath(line, error_page.second, _debugLine);
		_server.error_pages.insert(error_page);
	}
	else if (line.substr(0, 10) == "limit_size")
	{
		size_t		len = 0;
		std::string	spaces = WHITE_SPACES "#";

		line = line.c_str() + 10;
		skipWS(line);
		while (line[len] && spaces.find(line[len]) == std::string::npos)
			len++;
		_server.client_max_body_size = atoi(line.substr(0, len).c_str());
		line = line.c_str() + len;
		skipWS(line);
		if (line.length() && line[0] != '#')
			throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));   //debug mode (+)
	}
	else if (line.substr(0, 8) == "location")
	{
		_locationNameLine(line, is_location, is_inlocation);
	}
	else if (line.substr(0, 3) == "cgi")
	{
		size_t								len = 0;
		std::string							spaces = WHITE_SPACES "#";
		std::pair<std::string, std::string>	cgi;

		line = line.c_str() + 3;
		skipWS(line);
		while (line[len] && spaces.find(line[len]) == std::string::npos)
			len++;
		cgi.first = line.substr(0, len);
		line = line.c_str() + len;
		skipWS(line);
		if (!line.length() || line[0] == '#')
			throw std::logic_error("End of line in config file when waiting an argument line " + std::to_string(_debugLine));
		savePath(line, cgi.second, _debugLine);
		_server.cgi.insert(cgi);
	}
}

int	Config::checkAndParse(void)
{
	bool		is_server = false;
	bool		is_inserver = false;
	bool		is_location = false;
	bool		is_inlocation = false;
	std::string	line;

	_setDefaultServer();
	while (getline(_fileStream, line))
	{
		++_debugLine;
		skipWS(line);
		if (!line.length() || line[0] == '#')
			continue;
		if (!is_server && line.substr(0, 6) == "server")
			_serverNameLine(line, is_server, is_inserver);
		else if (!is_server)
			throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));   //debug mode (+)
		else if (line[0] == '{')
		{
			if (!is_inserver && is_server)
				checkisin(line, is_inserver, _debugLine);
			else if (!is_inlocation && is_location)
				checkisin(line, is_inlocation, _debugLine);
			else
				throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));   //debug mode (+)
		}
		else if (line[0] == '}')
		{
			if (is_inlocation)
				is_inlocation = false;
			else if (is_inserver)
				is_inserver = false;
			else
				throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));   //debug mode (+)
			line = line.c_str() + 1;
			skipWS(line);
			if (!is_inserver && (line.length() || line[0] == '#'))
				throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));   //debug mode (+)
		}
		else if (is_inlocation)
			_locationArgs(line);
		else
			_serverArgs(line, is_location, is_inlocation);
	}
	return 0;
}

t_server	&Config::get_server(void)
{
	return (_server);
}
