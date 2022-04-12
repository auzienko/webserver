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
  virtual ~NetworkConnection();
  int hasDataToReadEvent(void);
  int readyToAcceptDataEvent(void);
};

#endif
