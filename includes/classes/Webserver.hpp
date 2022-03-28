#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "../main.hpp"

class Config;
class Socket;
class RequestManager;

class Webserver {
 private:
  int _connectionCount;
  int _maxConnection;
  int _listenSocket;
  std::set<int> _connections;
  RequestManager* _rm;
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
  void appendSocketsToFdsSet(fd_set* fds, int* max_fd) const;
  int readHandler(int fd);
  int writeHandler(int fd);

 private:
  Webserver(Webserver const& src);
  Webserver& operator=(Webserver const& rhs);
};

#endif
