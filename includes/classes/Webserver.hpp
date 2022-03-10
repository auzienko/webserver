/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 11:41:06 by zcris             #+#    #+#             */
/*   Updated: 2022/03/10 10:22:30 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "../main.hpp"

class Config;
class Socket;

class Webserver {
 private:
  int _connectionCount;
  int _maxConnection;
  int _listenSocket;
  int _fd_max;
  fd_set _connections;

 public:
  Webserver(int maxConnection = MAX_CLIENTS);
  ~Webserver(void);
  int run(void);
  int const& getClientsCount(void) const;
  int createServerListenSocket(void);
  void plusConnection(void);
  void minusConnection(void);
  void addConnection(int fd);
  void closeConnection(int index);

 private:
  Webserver(Webserver const& src);
  Webserver& operator=(Webserver const& rhs);
};

#endif
