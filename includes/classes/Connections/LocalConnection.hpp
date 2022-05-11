#ifndef LOCALCONNECTION_HPP
#define LOCALCONNECTION_HPP

#include "classes/Connections/ConnectionManager.hpp"

class ConnectionManager;

class LocalConnection : public AConnection {
 private:
  LocalConnection(void);
  LocalConnection(LocalConnection const& src);
  LocalConnection& operator=(LocalConnection const& rhs);

  int _reading(void);
  int _writing(void);

 public:
  LocalConnection(ConnectionManager* cm, int fd, const std::map<int, std::string>* error_pages);
  virtual ~LocalConnection();
};

#endif
