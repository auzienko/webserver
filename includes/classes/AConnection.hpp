#ifndef ACONNECTION_HPP
#define ACONNECTION_HPP

#include "../main.hpp"
#include "ConnectionManager.hpp"
#include "UnknownNetworkTask.hpp"

enum ConnectionType { NETWORK_CONNECTION, LOCAL_CONNECTION };

class ATask;
class ConnectionManager;

class AConnection {
 protected:
  ConnectionManager* _connectionManager;
  int _subscriptionFd;
  int _sendResultFd;
  int _type;
  ATask* _task;
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
  void setTask(ATask* task);
  ATask* getTask(void) const;
  void killTask(void);
  void replaceTask(ATask* newTask);
  ConnectionManager* getConnectionManager(void) const;

  std::stringstream& getInputData(void);
  void addToOutput(std::string str);
  virtual int readData(void) = 0;
  virtual int handleData(void) = 0;
};

#endif
