#ifndef NETWORKCONNECTION_HPP
#define NETWORKCONNECTION_HPP

#include "ConnectionManager.hpp"

class ConnectionManager;

class NetworkConnection : public AConnection {
 private:
  NetworkConnection(void);
  NetworkConnection(NetworkConnection const& src);
  NetworkConnection& operator=(NetworkConnection const& rhs);

  int _reading(void);
  int _writing(void);

 public:
  NetworkConnection(ConnectionManager* cm, int inputFd, const std::map<int, std::string>* error_pages);
  virtual ~NetworkConnection();
};

#endif
