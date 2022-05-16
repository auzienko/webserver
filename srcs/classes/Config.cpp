#include "../../includes/classes/Config.hpp"

Config::Config(std::string const& file_name) : _configFile(file_name), _debugLine(0), _server(t_server())
{
	_fileStream.open(_configFile);
}

Config::~Config(void) {}

void	Config::close()
{
	_fileStream.close();
}

void	Config::_checkisin(std::string &line, bool &isIn)
{
	if (line[0] == '{')
	{
		isIn = true;
		line.erase(0, 1);
		ws::stringSkipWS(line);
		if (!line.length() || line[0] == '#')
			return ; 
		throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
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
	ws::stringSkipWS(line);
	if (line[0] == '{')
	{
		is_inserver = true;
		line = line.c_str() + 1;
		ws::stringSkipWS(line);
		if (line.length() && line[0] != '#')
			throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
		return ;
	}
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
	ws::stringSkipWS(line);
	if (!line.length() || line[0] == '#')
		return ;
	_checkisin(line, is_inserver);
	if (!line.length() || line[0] == '#')
		return ;
	throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
}

void	Config::_savePath(std::string &line, std::string &field, bool is_end)
{
	size_t		len = 0;
	std::string	spaces = WHITE_SPACES "#";

	while (line[len] && spaces.find(line[len]) == std::string::npos)
		len++;
	if (!len)
		throw std::logic_error("There are no argument when it's need in config file line " + std::to_string(_debugLine));
	field = line.substr(0, len);
	line = line.c_str() + len;
	ws::stringSkipWS(line);
	if (is_end && line.length() && line[0] != '#')
		throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
}

void	Config::_locationNameLine(std::string &line, bool &is_location, bool &is_inlocation)
{
	t_location	cur;
	size_t		len = 0;
	std::string	spaces = WHITE_SPACES "#";

	is_location = true;
	line = line.c_str() + 8;
	ws::stringSkipWS(line);
	if (!line.length() || line[0] == '#' || line[0] == '{')
	{
		throw std::logic_error("There are no argument when it's need in config file line " + std::to_string(_debugLine));
	}
	while (line[len] && spaces.find(line[len]) == std::string::npos)
		len++;
	cur.path = line.substr(0, len);
	line = line.c_str() + len;
	ws::stringSkipWS(line);
	_server.locations.push_back(cur);
	if (!line.length() || line[0] == '#')
		return ;
	_checkisin(line, is_inlocation);
	if (!line.length() || line[0] == '#')
		return ;
	throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
}

void	Config::_locationArgs(std::string &line)
{
	t_location	&cur = _server.locations.back();
	if (line.substr(0, 7) == "methods")
	{
		line = line.c_str() + 7;
		ws::stringSkipWS(line);
		if (!line.length())
			throw std::logic_error("End of line in config file when waiting an argument line " + std::to_string(_debugLine));
		while (line.length() && line[0] != '#')
		{
			size_t	len = 0;
			std::string	spaces = WHITE_SPACES "#";
			while (line[len] && spaces.find(line[len]) == std::string::npos)
				len++;
			cur.methods.insert(line.substr(0, len));
			line = line.c_str() + len;
			ws::stringSkipWS(line);
		}
	}
	else if (line.substr(0, 4) == "root")
	{
		line = line.c_str() + 4;
		ws::stringSkipWS(line);
		_savePath(line, cur.root);
	}
	else if (line.substr(0, 5) == "redir")
	{
		std::string	spaces = WHITE_SPACES "#";
		size_t len = 0;
		line = line.c_str() + 5;
		ws::stringSkipWS(line);
		cur.redir.code = atoi(line.c_str());
		while (line[len] && spaces.find(line[len]) == std::string::npos)
			len++;
		if (!len)
			throw std::logic_error("End of line in config file when waiting an argument line " + std::to_string(_debugLine));
		line.erase(0, len);
		ws::stringSkipWS(line);
		_savePath(line, cur.redir.target);
	}
	else if (line.substr(0, 9) == "autoindex")
	{
		line = line.c_str() + 9;
		ws::stringSkipWS(line);
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
		else
			throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
		ws::stringSkipWS(line);
		if (line.length() && line[0] != '#')
			throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
	}
	else if (line.substr(0, 10) == "limit_size")
	{
		size_t		len = 0;
		std::string	spaces = WHITE_SPACES "#";

		line = line.c_str() + 10;
		ws::stringSkipWS(line);
		while (line[len] && spaces.find(line[len]) == std::string::npos)
			len++;
		if (!len)
			throw std::logic_error("End of line in config file when waiting an argument line " + std::to_string(_debugLine));
		cur.client_max_body_size = atoi(line.substr(0, len).c_str());
		line = line.c_str() + len;
		ws::stringSkipWS(line);
		if (line.length() && line[0] != '#')
			throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
	}
	else if (line.substr(0, 5) == "index")
	{
		line = line.c_str() + 5;
		ws::stringSkipWS(line);
		_savePath(line, cur.index);
	}
	else if (line.substr(0, 6) == "upload")
	{
		line = line.c_str() + 6;
		ws::stringSkipWS(line);
		_savePath(line, cur.uploads_path);
		cur.file_uploads = true;
	}
	else
		throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
}

void	Config::_serverArgs(std::string &line, bool &is_location, bool &is_inlocation)
{
	if (!_server.port)
	{
		size_t		len = 0;
		std::string	spaces = WHITE_SPACES "#";

		while (line[len] && line[len] != ':')
			len++;
		if (!len)
			throw std::logic_error("There is no host in config file line " + std::to_string(_debugLine));
		_server.listen = line.substr(0, len);
		line = line.c_str() + len + 1;
		len = 0;

		while (line[len] && spaces.find(line[len]) == std::string::npos)
			len++;
		if (!len)
			throw std::logic_error("End of line in config file when waiting an argument line " + std::to_string(_debugLine));
		_server.port = atoi(line.substr(0, len).c_str());
		if (!_server.port)
			throw std::logic_error("1 line of server properties must be host:port. Error in line " + std::to_string(_debugLine));
		line = line.c_str() + len;
		ws::stringSkipWS(line);
		if (line.length() && line[0] != '#')
			throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
	}
	else if (line.substr(0, 10) == "error_page")
	{
		size_t						len = 0;
		std::string					spaces = WHITE_SPACES "#";
		std::pair<int, std::string>	error_page;

		line = line.c_str() + 10;
		ws::stringSkipWS(line);
		while (line[len] && spaces.find(line[len]) == std::string::npos)
			len++;
		if (!len)
			throw std::logic_error("End of line in config file when waiting an argument line " + std::to_string(_debugLine));
		error_page.first = atoi(line.substr(0, len).c_str());
		line = line.c_str() + len;
		ws::stringSkipWS(line);
		if (!line.length() || line[0] == '#')
			throw std::logic_error("End of line in config file when waiting an argument line " + std::to_string(_debugLine));
		_savePath(line, error_page.second);
		_server.error_pages.insert(error_page);
	}
	else if (line.substr(0, 10) == "limit_size")
	{
		size_t		len = 0;
		std::string	spaces = WHITE_SPACES "#";

		line = line.c_str() + 10;
		ws::stringSkipWS(line);
		while (line[len] && spaces.find(line[len]) == std::string::npos)
			len++;
		if (!len)
			throw std::logic_error("End of line in config file when waiting an argument line " + std::to_string(_debugLine));
		_server.client_max_body_size = atoi(line.substr(0, len).c_str());
		line = line.c_str() + len;
		ws::stringSkipWS(line);
		if (line.length() && line[0] != '#')
			throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
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
		std::pair<std::string, std::set<std::string> >	cgi_methods;
		std::set<std::string>	methods;

		line = line.c_str() + 3;
		ws::stringSkipWS(line);
		while (line[len] && spaces.find(line[len]) == std::string::npos)
			len++;
		if (!len)
			throw std::logic_error("End of line in config file when waiting an argument line " + std::to_string(_debugLine));
		cgi.first = line.substr(0, len);
		cgi_methods.first = line.substr(0, len);
		line = line.c_str() + len;
		ws::stringSkipWS(line);
		if (!line.length() || line[0] == '#')
			throw std::logic_error("End of line in config file when waiting an argument line " + std::to_string(_debugLine));
		_savePath(line, cgi.second, false);
		_server.cgi.insert(cgi);
		line = line.c_str() + cgi.second.length();
		ws::stringSkipWS(line);
		if (!line.length())
			cgi_methods.second = methods;
		else
		{
			while (line.length() && line[0] != '#')
			{
				size_t	len = 0;  // переопределение затенение переменной len
				std::string	spaces = WHITE_SPACES "#"; //аналогично с spaces
				while (line[len] && spaces.find(line[len]) == std::string::npos)
					len++;
				methods.insert(line.substr(0, len));
				line = line.c_str() + len;
				ws::stringSkipWS(line);
			}
			cgi_methods.second = methods;
		}
		_server.cgi_methods.insert(cgi_methods);
	}
	else
		throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
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
		ws::stringSkipWS(line);
		if (!line.length() || line[0] == '#')
			continue;
		if (!is_server && line.substr(0, 6) == "server")
			_serverNameLine(line, is_server, is_inserver);
		else if (!is_server)
			throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
		else if (line[0] == '{')
		{
			if (!is_inserver && is_server)  // is_server всегда true
				_checkisin(line, is_inserver);
			else if (!is_inlocation && is_location)
				_checkisin(line, is_inlocation);
			else
				throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
		}
		else if (line[0] == '}')
		{
			if (is_inlocation)
			{
				is_inlocation = false;
				is_location = false;
			}
			else if (is_inserver)
			{
				is_inserver = false;
				is_server = false;
				return (1);
			}
			else
				throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
			line = line.c_str() + 1;
			ws::stringSkipWS(line);
			if (!is_inserver && (line.length() || line[0] == '#'))
				throw std::logic_error("Find unsuspected char in config file line " + std::to_string(_debugLine));
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
