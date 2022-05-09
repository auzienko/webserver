#ifndef NETWORKCONNECTION_HPP
#define NETWORKCONNECTION_HPP

#include "classes/Connections/ConnectionManager.hpp"

class ConnectionManager;

class NetworkConnection : public AConnection {
 private:
  ssize_t _wrote;
  NetworkConnection(void);
  NetworkConnection(NetworkConnection const& src);
  NetworkConnection& operator=(NetworkConnection const& rhs);

 public:
  NetworkConnection(ConnectionManager* cm, int inputFd, const std::map<int, std::string>* error_pages);
  virtual ~NetworkConnection();
  int hasDataToReadEvent(void);
  int readyToAcceptDataEvent(void);
};

#endif
