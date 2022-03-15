#ifndef REQUEST_MANAGER_HPP
#define REQUEST_MANAGER_HPP

#include "../main.hpp"

class Request;

class Request_manager {
 private:
  std::map<int, Request*> _list;

 public:
  Request_manager();
  ~Request_manager(); //реализовать, там зачистка с указателей

  void add(int fd);
  void remove(int fd);
  Request* at(int fd) const;
  int getRequest(int fd, t_server const& server_config);
  int sendResult(int fd);
  
};

#endif
