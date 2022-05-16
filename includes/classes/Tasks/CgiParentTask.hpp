#ifndef CGIPARENTTASK_HPP
#define CGIPARENTTASK_HPP

#include "../Connections/AConnection.hpp"
#include "ATask.hpp"
class AConnection;

class CgiParentTask : public ATask {
 private:
  std::stringstream _Header;
  std::stringstream _Body;
  std::stringstream _response;
  int const _childInputFd;
  int const _childOutputFd;
  int const _pid;

  CgiParentTask(void);
  CgiParentTask(CgiParentTask const& src);
  CgiParentTask& operator=(CgiParentTask const& rhs);

 public:
  CgiParentTask(AConnection* connection, int const& fd, int const& childInputFd,
                int const& childOutputFd, int pid);
  virtual ~CgiParentTask();
  int collectData(void);
  int executeTask(void);
  int sendResult(void);
};

#endif
