#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "../Config.hpp"
#include "../Connections/ConnectionManager.hpp"

class Config;
class ConnectionManager;

class Webserver {
 private:
  int _maxConnection;
  int _listenSocket;
  ConnectionManager* _connectionManager;
  t_server _serverConfig;

 public:
  Webserver(t_server &src, int maxConnection = MAX_CLIENTS);
  ~Webserver(void);
  int run(void);
  int getClientsCount(void) const;
  int createServerListenSocket(void);
  void addConnection(int fd);
  void closeConnection(int index);
  void makeActiveFdsSet(fd_set* fds, int* max_fd) const;
  int readHandler(int fd);
  int otherHandler(int fd);
  void closeConnectionIfTimout(int seconds);
  void closeConnectionIfDone();
  t_server const& getServerConfig(void) const;

 private:
  Webserver(Webserver const& src);
  Webserver& operator=(Webserver const& rhs);
};

#endif
