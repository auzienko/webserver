#ifndef ACONNECTION_HPP
#define ACONNECTION_HPP

#include "../main.hpp"
#include "ConnectionManager.hpp"
#include "Request.hpp"

enum ConnectionType { NETWORK_CONNECTION, LOCAL_CONNECTION };

class Request;
class ConnectionManager;

class AConnection {
 protected:
  ConnectionManager* _connectionManager;
  int _subscriptionFd;
  int _sendResultFd;
  int _type;
  Request* _task;
  std::time_t _lastActivity;
  std::stringstream _input;
  std::stringstream _output;

 private:
  AConnection(AConnection const& src);
  AConnection& operator=(AConnection const& rhs);

 public:
  AConnection(void);
  AConnection(ConnectionManager* cm, int inputFd);
  AConnection(ConnectionManager* cm, int inputFd, int outputFd);
  virtual ~AConnection(void);
  void setLastActivity(void);
  std::time_t getLastActivity(void) const;

  int getFd(void);
  void setTask(Request* task);
  Request* getTask(void) const;
  void killTask(void);
  ConnectionManager* getConnectionManager(void) const;

  void addToOutput(std::string str);
  int readData(t_server const& server_config);
  virtual int sendData(void) = 0;
};

#endif
