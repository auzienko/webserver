/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver_manager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wgaunt <wgaunt@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 07:44:23 by zcris             #+#    #+#             */
/*   Updated: 2022/03/10 18:30:48 by wgaunt           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_MANAGER_HPP
#define WEBSERVER_MANAGER_HPP

#include "../main.hpp"

class Config;
class Webserver;

class Webserver_manager {
 private:
  Config* _config;
  std::vector<Webserver*> _webservers;

 public:
  Webserver_manager(std::string const& config_path);
  ~Webserver_manager();
  int start(void);

 private:
  Webserver_manager(void);
  Webserver_manager(Webserver_manager const& src);
  Webserver_manager& operator=(Webserver_manager const& rhs);
  int _Create_webserver(t_server &newServerConfig);
  void _Banner(void) const;
};

#endif
