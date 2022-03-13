#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "../main.hpp"

class Config;
class Socket;
class Request_manager;

class Webserver {
 private:
  int _connectionCount;
  int _maxConnection;
  int _listenSocket;
  int _maxFd;
  fd_set _connections;
  Request_manager* _rm;
  t_server _serverConfig;

 public:
  Webserver(t_server &src, int maxConnection = MAX_CLIENTS);
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
  void _ReadHandler(int fd);
  void _WriteHandler(int fd);
};

#endif
