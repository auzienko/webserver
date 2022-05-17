#ifndef CGIOUTPUTTASK_HPP
#define CGIOUTPUTTASK_HPP

#include "classes/Connections/AConnection.hpp"
#include "ATask.hpp"
class AConnection;

class CgiOutputTask : public ATask {
 private:
  int const _parentFd;
  int _length;

  CgiOutputTask(void);
  CgiOutputTask(CgiOutputTask const& src);
  CgiOutputTask& operator=(CgiOutputTask const& rhs);

 public:
  CgiOutputTask(AConnection* connection, int const& fd, int const& parentFd);
  virtual ~CgiOutputTask();
  int collectData(void);
  int executeTask(void);
  int sendResult(void);
  void addLength(int addedLength);
  int setLastActivity(void);
};

#endif
