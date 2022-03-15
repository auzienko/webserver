#ifndef REQUESTMANAGER_HPP
#define REQUESTMANAGER_HPP

#include "../main.hpp"

class Request;

class RequestManager {
 private:
  std::map<int, Request*> _list;

 public:
  RequestManager();
  ~RequestManager(); //реализовать, там зачистка с указателей

  void add(int fd);
  void remove(int fd);
  Request* at(int fd) const;
  int getRequest(int fd, t_server const& server_config);
  int sendResult(int fd);
  
};

#endif
