#ifndef CGIOUTPUTTASK_HPP
#define CGIOUTPUTTASK_HPP

#include "classes/Connections/AConnection.hpp"
#include "classes/Tasks/ATask.hpp"
class AConnection;

class CgiOutputTask : public ATask {
 private:
  int const _parentFd;

  CgiOutputTask(void);
  CgiOutputTask(CgiOutputTask const& src);
  CgiOutputTask& operator=(CgiOutputTask const& rhs);

 public:
  CgiOutputTask(AConnection* connection, int const& fd, int const& parentFd);
  virtual ~CgiOutputTask();
  int collectData(void);
  int executeTask(void);
  int sendResult(void);
  int setLastActivity(void);
};

#endif
