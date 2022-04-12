#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include "../../main.hpp"
#include "AConnection.hpp"

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
  int readData(int fd);
  int handleData(int fd);
  std::vector<int> getAllConnectionsFds(void) const;
  int getConnectionCount(void) const;

  void closeConnectionIfTimout(int seconds);
};

#endif
