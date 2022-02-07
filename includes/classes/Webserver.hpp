/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 11:41:06 by zcris             #+#    #+#             */
/*   Updated: 2022/02/07 17:21:56 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "../main.hpp"

class Config;
class Socket;

class Webserver {
 private:
  Config* _config;
  int _connectionCount;
  int _maxConnection;
  std::vector<int> _listenSockets;
  std::vector<pollfd> _connections;

  Webserver(void);
  Webserver(Webserver const& src);
  Webserver& operator=(Webserver const& rhs);

 public:
  Webserver(Config* config, int maxConnection = MAX_CLIENTS);
  ~Webserver(void);
  void banner(void) const;
  int run(void);
  int const& getClientsCount(void) const;
  int createServerListenSockets(void);
  void plusConnection(void);
  void minusConnection(void);
  void addConnection(int fd, short events);
  void closeConnection(int index);
  int sendResult(int fd, char* buf);
  int getRequest(int fd, char* buf);
};

#endif
