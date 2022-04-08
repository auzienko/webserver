#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include "../main.hpp"

class Connection;

class ConnectionManager {
 private:
  std::map<int, Connection*> _list;

 public:
  ConnectionManager();
  ~ConnectionManager(); //реализовать, там зачистка с указателей

  int isExist(int const fd) const;
  void add(int fd);
  void add(int fd, int parentFd);
  void remove(int fd);
  Connection* at(int fd) const;
  int getRequest(int fd, t_server const& server_config);
  int sendResult(int fd);
  std::vector<int> getAllConnectionsFds(void) const;
  int getConnectionCount(void) const;

  void closeConnectionIfTimout(int seconds);
};

#endif
