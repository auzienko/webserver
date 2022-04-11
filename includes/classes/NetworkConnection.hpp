#ifndef NETWORKCONNECTION_HPP
#define NETWORKCONNECTION_HPP

#include "ConnectionManager.hpp"

class ConnectionManager;

class NetworkConnection : public AConnection {
 private:
  NetworkConnection(void);
  NetworkConnection(NetworkConnection const& src);
  NetworkConnection& operator=(NetworkConnection const& rhs);

 public:
  NetworkConnection(ConnectionManager* cm, int inputFd);
  NetworkConnection(ConnectionManager* cm, int inputFd, int outputFd);
  virtual ~NetworkConnection();
  int readData(void);
  int handleData(void);
};

#endif
