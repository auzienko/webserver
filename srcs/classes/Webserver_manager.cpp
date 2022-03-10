/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver_manager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 07:49:54 by zcris             #+#    #+#             */
/*   Updated: 2022/03/10 08:55:14 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/classes/Webserver_manager.hpp"

Webserver_manager::Webserver_manager(std::string const& config_path) {
  _config = new Config(config_path);
  if (_config->checkAndParse() < 0) {
    delete _config;
    throw new std::exception;
  } else {
    _Create_webservers();
  }
}

int Webserver_manager::start(void) {
  _Banner();
  //пока один
  _webservers[0]->run();
  return 0;
}

Webserver_manager::~Webserver_manager() {
  delete _config;
  std::vector<Webserver*>::iterator i = _webservers.begin();
  std::vector<Webserver*>::iterator e = _webservers.end();
  for (; i != e; ++i) delete (*i);
}

int Webserver_manager::_Create_webservers(void) {
  _webservers.push_back(new Webserver());
  return 0;
}

void Webserver_manager::_Banner(void) const {
  ws::print("==================================", "\n");
  ws::print(PROGRAMM_NAME, " ");
  ws::print(PROGRAMM_VERSION, "\n");
  ws::print(AUTHORS, "\n");
  ws::print("==================================", "\n");
}
