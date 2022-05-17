#ifndef ACONNECTION_HPP
#define ACONNECTION_HPP

#include "main.hpp"
#include "ConnectionManager.hpp"
#include "classes/Tasks/ATask.hpp"

class ATask;
class ConnectionManager;

class AConnection {
 protected:
  ssize_t _wrote;
  std::string::size_type _len;
  char _buf[DEFAULT_BUFLEN];
  ConnectionManager* _connectionManager;
  int _idFd;
  ATask* _task;
  long int _lastActivity;
  std::stringstream _input;
  std::stringstream _output;
  const std::map<int, std::string>* _error_pages;

  virtual int _reading(void) = 0;
  virtual int _writing(void) = 0;

 private:
  AConnection(AConnection const& src);
  AConnection& operator=(AConnection const& rhs);

 public:
  AConnection(void);
  AConnection(ConnectionManager* cm, int fd, const std::map<int, std::string>* error_pages);
  virtual ~AConnection(void);
  void setLastActivity(void);
  long int getLastActivity(void) const;

  int io();
  void error(const std::exception &ex);
  int getFd(void);
  void setTask(ATask* task);
  ATask* getTask(void) const;
  void killTask(void);
  void replaceTask(ATask* newTask);
  ConnectionManager* getConnectionManager(void) const;

  std::stringstream& getInputData(void);
  void addToOutput(std::string const& str);
};

#endif
