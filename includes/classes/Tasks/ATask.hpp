#ifndef ATASK_HPP
#define ATASK_HPP

enum Status {
  NEW,
  READING,
  READY_TO_HANDLE,
  EXECUTION,
  READY_TO_SEND,
  SENDING,
  DONE
};
enum Type {
  UNKNOWN_NETWORK,
  NETWORK_AUTOINDEX,
  NETWORK_GET,
  NETWORK_POST,
  NETWORK_REDIRECT,
  NETWORK_DELETE,
  NETWORK_CGI_PARENT,
  NETWORK_HEAD,
  LOCAL_CGI_INPUT,
  LOCAL_CGI_OUTPUT
};

#include "../Connections/AConnection.hpp"

class AConnection;

class ATask {
 protected:
  AConnection* _connection;
  std::string _resBodyType;
  bool _isKeepAlive;

 private:
  int _fd;
  int _status;
  int _type;
  ATask(ATask const& src);
  ATask& operator=(ATask const& rhs);
  ATask(void);

 public:
  ATask(int type, int fd, AConnection* connection);
  virtual ~ATask();
  int getStatus(void) const;
  void setStatus(int status);
  int getType(void) const;
  bool getIsKeepAlive(void) const;
  int getFd(void) const;
  int doTask(void);
  virtual int setLastActivity(void);
  virtual int collectData(void) = 0;
  virtual int executeTask(void) = 0;
  virtual int sendResult(void) = 0;
};
#endif
