#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include "main.hpp"
#include "classes/Connections/AConnection.hpp"
#include "classes/Webservers/Webserver.hpp"

class AConnection;
class Webserver;

class ConnectionManager {
 private:
  std::map<int, AConnection*> _list;
  Webserver* _webserver;

 public:
  ConnectionManager(Webserver* webserver);
  ~ConnectionManager();

  int isExist(int const fd) const;
  void add(AConnection* connection);
  void remove(int fd);
  AConnection* at(int fd) const;
  int hasDataToReadEvent(int fd);
  int readyToAcceptDataEvent(int fd);
  std::vector<int> getAllConnectionsFds(void) const;
  int getConnectionCount(void) const;
  Webserver const* getWebserver(void) const;

  void closeConnectionIfTimout(int seconds);
};

#endif
