/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wgaunt <wgaunt@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 10:55:56 by zcris             #+#    #+#             */
/*   Updated: 2022/03/11 14:34:45 by wgaunt           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/classes/Config.hpp"

Config::Config(std::string const& file_name) : _config_file(file_name)
{
	_file_stream.open(_config_file);
}

Config::~Config(void) {}

static void	skipWS(std::string &line)
{
	const char	*ptr = line.c_str();
	std::string	spaces = WHITE_SPACES;

	while (spaces.find(*ptr))
	{
		ptr++;
	}
	line = ptr;
}

void	Config::_setDefaultServer()
{
	_server.cgi_extention.clear();
	_server.cgi_path.clear();
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
	if (!line.size() || line[0] == '#' || line[0] == '{')
		return ;
	const char	*ptr = line.c_str();
	std::string	spaces = std::strcat(WHITE_SPACES, "#{");
	size_t		len = 0;
	while (ptr && !spaces.find(*ptr))
	{
		len++;
		ptr++;
	}
	_server.server_name = line.substr(0, len);
	skipWS(line);
	if (!line.size() || line[0] == '#')
		return ;
	if (line[0] == '{')
	{
		is_inserver = true;
		skipWS(line);
		if (!line.size() || line[0] == '#')
			return ;
		throw std::logic_error("Find unsuspected char in config file " + line);   //debug mode (+)
	}
	throw std::logic_error("Find unsuspected char in config file " + line);   //debug mode (+)
}

int	Config::checkAndParse(void)
{
	bool		is_server = false;
	bool		is_inserver = false;
	bool		is_location = false;
	bool		is_inlocation = false;
	std::string	line;

	_setDefaultServer();
	while (getline(_file_stream, line))
	{
		skipWS(line);
		if (!line.size() || line[0] == '#')
			continue;
		if (!is_server && line.substr(0, 6) == "server")
			_serverNameLine(line, is_server, is_inserver);
	}
	return 0;
}

t_server	&Config::get_server(void)
{
	return (_server);
}