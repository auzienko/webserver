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
  LocalConnection(ConnectionManager* cm, int fd);
  virtual ~LocalConnection();
  int hasDataToReadEvent(void);
  int readyToAcceptDataEvent(void);
};

#endif
