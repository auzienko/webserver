#ifndef LOCALCONNECTION_HPP
#define LOCALCONNECTION_HPP

#include "ConnectionManager.hpp"

class ConnectionManager;

class LocalConnection : public AConnection {
 private:
  LocalConnection(void);
  LocalConnection(LocalConnection const& src);
  LocalConnection& operator=(LocalConnection const& rhs);

 public:
  LocalConnection(ConnectionManager* cm, int inputFd);
  LocalConnection(ConnectionManager* cm, int inputFd, int outputFd);
  virtual ~LocalConnection();
  int sendData(void);
};

#endif
