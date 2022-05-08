#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include "main.hpp"
#include "classes/Connections/AConnection.hpp"

class AConnection;

class ConnectionManager {
 private:
  std::map<int, AConnection*> _list;

 public:
  ConnectionManager();
  ~ConnectionManager();

  int isExist(int const fd) const;
  void add(AConnection* connection);
  void remove(int fd);
  AConnection* at(int fd) const;
  int hasDataToReadEvent(int fd);
  int readyToAcceptDataEvent(int fd);
  std::vector<int> getAllConnectionsFds(void) const;
  int getConnectionCount(void) const;

  void closeConnectionIfTimout(int seconds);
};

#endif
